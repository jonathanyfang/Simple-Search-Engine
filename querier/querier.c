/*
* querier.c - CS50 'querier'
* 
* usage: ./querier pageDirectory indexFilename
* pageDirectory is the pathname of a directory produced by the Crawler
* indexFilename is the pathname of a file produced by the Indexer
* 
* Jonathan Fang
* CS50, Fall 2022
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../libcs50/file.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"

const int QUERY_LIM = 100;
static void parseArgs(int argc, char *argv[]);
static void querier(char* pageDirectory, char* indexFilename, index_t *index);
int fileno(FILE *stream); // given in instructions
static void prompt(void); // given in instructions
static void score_query(char* tokens[], index_t *index, int word_count, char* pageDirectory);
static void counters_merge(counters_t* result, counters_t* another);
static void counters_merge_helper(void* arg, const int key, const int item);
static void counters_intersect(counters_t* ct1, counters_t* ct2); 
static void intersect_helper(void *arg, const int key, const int count);
static void match_count(void* arg, const int key, int item);
static void get_ranking(void* arg, const int docID, int score);

/******** local data types *******/
// modified from iterable given
typedef struct twocts {
  counters_t *result;
  counters_t *another;
} twocts_t;

typedef struct id_and_score {
  int id;
  int score;
} id_and_score_t;

// Main
// usage: ./querier pageDirectory indexFilename
int main (const int argc, char *argv[]) {
    parseArgs(argc, argv);
    index_t *index = index_load(argv[2]);
    querier(argv[1], argv[2], index);
    index_delete(index);
    return 0;
}

// Parse through the arguments
static void parseArgs(int argc, char *argv[]) {
    FILE *fp;
    // Check usage
    if (argc != 3) {
        fprintf(stderr, "usage: ./querier crawlerDirectory indexFilename\n");
        exit(1);
    }
    // Check if crawler directory and if it can be read. Will print to stderr on its own
    else if (!pagedir_validate(argv[1])) {
        exit(2);
    }
    // Check if indexFilename is readable
    else if ((fp = fopen(argv[2], "r")) == NULL) {
        fprintf(stderr, "indexFilename is not readable\n");
        exit(3);
    }
    fclose(fp); // clean up
}

// The function where our querier will be contained
static void querier(char* pageDirectory, char* indexFilename, index_t* index) {
    // Begin reading from stdin
    char* line;
    FILE *input = stdin;
    char* tokens[QUERY_LIM];
    prompt();
    while((line = file_readLine(input)) != NULL) {
        int word_count = 0;
        // won't enter this if statement if line is empty, if non alph, or invalid query, instead continue
        if (tokenizeString(line, tokens, &word_count) && validateQuery(tokens, word_count)) {
            // Print the normalized query
            printf("Query: ");
            for (int i = 0; i < word_count; i++) {
                printf(" %s", tokens[i]);
            }
            printf("\n");
            // Begin to score the query
            score_query(tokens, index, word_count, pageDirectory);
        }
        free(line);
        prompt();
        // clean up tokens
        for (int i = 0; i < word_count; i++ ) {
            tokens[i] = '\0';
        }
    }
}

// Only prints when stdin is a user, not during testing
static void prompt(void) {
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

static void score_query(char* tokens[], index_t *index, int word_count, char* pageDirectory) {
    counters_t *result = counters_new();
    counters_t *temp = NULL;
    for (int i = 0; i < word_count; i++) {
        if (!isOperator(tokens[i])) {
            counters_t* ctrs = index_find(index, tokens[i]);
            if (temp == NULL) {
                temp = counters_new();
                counters_merge(temp, ctrs);   //first word in AND sequence. ctrs is unchanged, so no need to copy it
            }
            else {
                counters_intersect(temp, ctrs);  //intersect on AND. ctrs is unchanged, so no need to copy it
            }
        }
        else if(strcmp(tokens[i], "or") == 0) {
            counters_merge(result, temp); //union on OR
            counters_delete(temp);
            temp = NULL;
        }
        else if(strcmp(tokens[i], "and") == 0) {
             continue; //implicit AND between words
        }
    }
    counters_merge(result, temp); // result is the merged result
    int num = 0;
    counters_iterate(result, &num, match_count);
    if (num == 0){
        printf("No documents match.\n");
        printf("-----------------------------------------------\n");
        counters_delete(temp);
        counters_delete(result);
        return;
    }
    // If there are some matches, print a statement
    printf("Matches %d documents (ranked):\n", num);
    for (int i = 0; i < num; i++) {
        // Initialize the id_and_score struct
        id_and_score_t *top = malloc(sizeof(id_and_score_t));
        top->id = 0;
        top->score = 0;
        // iterate and get the rankings, which updates top
        counters_iterate(result, top, get_ranking);
        int docID = top->id;
        int score = top->score;

        // Use docID to get url. Assume there won't be more than 4 docID digits
        int len = strlen(pageDirectory) + 5;
        char *filename = malloc(sizeof(char) * len);
        // convert id to str and concat to pageDirectory. Assume first docID starts at 1
        int ID = 1;
        sprintf(filename, "%s/%d", pageDirectory, ID);

        // infd url and print
        FILE* fp = fopen(filename, "r");
        char* url = file_readLine(fp);
        printf("score %3d doc %3d: %s\n", score, docID, url);
        counters_set(result, docID, 0); // Set the max score to 0 once printed
        // clean up
        fclose(fp);
        free(url);
        free(filename);
        free(top);
    }
    printf("-----------------------------------------------\n");
    counters_delete(temp);
    counters_delete(result);
    return;
}

/* Merge the second counterset into the first counterset;
 * the second counterset is unchanged. Adapted from the set_merge given
 */
static void counters_merge(counters_t* result, counters_t* another) {
  counters_iterate(another, result, counters_merge_helper);
}

/* Consider one item for insertion into the other set.
 * If the other set does not contain the item, insert it;
 * otherwise, update the other set's item with sum of item values.
 * Adapted from the set_merge_helper given
 */
static void counters_merge_helper(void* arg, const int key, const int count) {
  counters_t* result = arg;
  // find the same key in setA
  int count_result = counters_get(result, key);
  if (count_result == 0) {
    // not found: insert it
    counters_set(result, key, count);
  } 
  else {
    // add to the existing value
    counters_set(result, key, count_result + count);
  }
}


// counters_intersect modified from the instructions
static void counters_intersect(counters_t* ct1, counters_t* ct2) {
  twocts_t *args = malloc(sizeof(twocts_t));
  args->result = ct1;
  args->another = ct2;
  counters_iterate(ct1, args, intersect_helper);
  free(args);
}

// helper function for counters_iterate, modified from pseudocode given in instructions
static void intersect_helper(void *arg, const int key, const int count) {
    twocts_t *two = arg; 
    int other_count;
    if ((other_count = counters_get(two->another, key)) != 0) {
        int min = count < other_count ? count : other_count;
        counters_set(two->result, key, min);
    }
    else {
        counters_set(two->result, key, 0);
    }
}

// Used to count the matches using the counters iterable. adapted from given function
static void match_count(void* arg, const int key, int item) {
    int* num = arg;
    if (arg != NULL && item != 0) {
        (*num)++;
    }
}

// print the scores, rankings, and doc details
static void get_ranking(void* arg, const int docID, int score) {
    if (arg != NULL && score != 0) {
    id_and_score_t* ans = arg;
    if (score > ans->score) {
        ans->id = docID;
        ans->score = score;
    }
  }
}