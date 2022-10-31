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
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"

const int QUERY_LIM = 100;
static void parseArgs(int argc, char *argv[]);
static void querier(char* pageDirectory, char* indexFilename);
int fileno(FILE *stream); // given in instructions
static void prompt(void); // given in instructions
static counters_t *score_query(char* tokens[], index_t *index);
static void counters_merge(counters_t* result, counters_t* another);
static void counters_merge_helper(void* arg, const char* key, void* item);
void counters_intersect(counters_t* ct1, counters_t* ct2); 
void intersect_helper(void *arg, const int key, const int count);

/******** local data types *******/
// modified from iterable given
typedef struct twocts {
  counters_t *result;
  counters_t *another;
} twocts_t;

// Main
int main (const int argc, char *argv[]) {
    parseArgs(argc, *argv);
    querier(argv[1], argv[2]);
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
static void querier(char* pageDirectory, char* indexFilename) {
    index_t *index = index_load(indexFilename);
    // Begin reading from stdin
    char* line;
    FILE *input = stdin;
    char* tokens[QUERY_LIM];
    prompt();
    while((line = freadlinep(input)) != NULL) {
        int word_count = 0;
        // won't enter this if statement if line is empty, if non alph, or invalid query, instead continue
        if (tokenizeString(line, tokens, &word_count)) {
            // Print the normalized query
            printf("Query: ");
            for (int i = 0; i < word_count; i++) {
                printf(" %s", tokens[i]);
            }
            printf("\n");
            // Begin to score the query
            counters_t *count = scoreQuery(tokens, index);
        }
        // else move on to next query
        else {
            printf("-----------------------------------------------\n");
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

static counters_t *score_query(char* tokens[], index_t *index) {

}

/* Merge the second counterset into the first counterset;
 * the second counterset is unchanged. Adapted from the set_merge given
 */
static void counters_merge(counters_t* result, counters_t* another) {
  set_iterate(another, result, counters_merge_helper);
}

/* Consider one item for insertion into the other set.
 * If the other set does not contain the item, insert it;
 * otherwise, update the other set's item with sum of item values.
 * Adapted from the set_merge_helper given
 */
static void counters_merge_helper(void* arg, const char* key, void* count) {
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
void counters_intersect(counters_t* ct1, counters_t* ct2) {
  mem_assert(ct1, "counters 1 invalid");
  mem_assert(ct2, "counters 2 invalid");
  twocts_t *args = malloc(sizeof(twocts_t));
  args->result = ct1;
  args->another = ct2;
  counters_iterate(ct1, &args, intersect_helper);
}

// helper function for counters_iterate, modified from pseudocode given in instructions
void intersect_helper(void *arg, const int key, const int count) {
  struct twocts *two = arg; 
  int other_count;
  if ((other_count = counters_get(two->another, key)) != 0) {
      int min = count < other_count ? count : other_count;
      counters_set(two->result, key, min);
  }
  else {
      counters_set(two->result, key, 0);
  }
  
}

// Used to count the matches using the counters iterable
static void match_count(void* arg, const char* key, void* item)
{
  if (arg != NULL && item != NULL) {
    int* num = arg;
    num++;
  }
}