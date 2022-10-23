/*
* index.h - 'CS50' index module
* 
* contains all the functions for the index data structure
* can initialize, insert into, set counts for, save, load, and delete indexes
* 
* Jonathan Fang
* CS50 Fall 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"

const int ht_size = 500;
static void hashtable_item_save(void* arg, const char* key, void* item);
static void counters_item_save(void* arg, const int key, const int count);
static void void_counters_delete(void* item);

/**************** global types ****************/
typedef struct index {
  hashtable_t* ht;       
} index_t;

/**************** index_new() ****************/
/* creates a new, empty index */
index_t *index_new(int ht_size) {
    index_t *index = malloc(sizeof(index_t)); // allocate memory
    if (index == NULL) {
        return NULL;              // error allocating index
    } 
    else {
        // initialize contents of index structure
        index->ht = hashtable_new(ht_size);
        return index;
    }
}

/**************** index_insert() ****************/
/* given a word and a docID, inserts the word into the index */
bool index_insert(index_t *index, const char* word, const int docID) {
    counters_t *ctrs;
    if (index != NULL && word != NULL && docID >= 0) { // check parameters
        if ((ctrs = hashtable_find(index->ht, word)) != NULL) { // if hashtable_find does not return null, word already existed as a key
            counters_add(ctrs, docID);
        }
        // word didn't exist as a key
        else {
            ctrs = counters_new();
            if (ctrs == NULL) { // check that ctrs is allocated properly
                fprintf(stderr, "error allocating the counterset\n");
                return false;
            }
            counters_add(ctrs, docID);
            hashtable_insert(index->ht, word, ctrs);
        }
        return true;
    }
    return false;
}

/**************** index_set() ****************/
/* given a word, docID, and count, finds the counter associated with the word, and sets the count associated with the docID */
bool index_set(index_t *index, const char* word, const int docID, const int count) {
    counters_t *ctrs;
    if (index != NULL && word != NULL && docID >= 0) { // check parameters
        if ((ctrs = hashtable_find(index->ht, word)) != NULL) { // if hashtable_find does not return null, word already existed as a key
            counters_set(ctrs, docID, count);
        }
        // word already existed as a key
        else {
            ctrs = counters_new();
            if (ctrs == NULL) { // check that ctrs is allocated properly
                fprintf(stderr, "error allocating the counterset\n");
                return false;
            }
            counters_set(ctrs, docID, count);
            hashtable_insert(index->ht, word, ctrs);
        }
        return true;
    }
    return false;
}

/**************** index_save() ****************/
/* saves the index to a file */
void index_save(index_t *index, const char* filename) {
  FILE *fp;
  // open the file and begin iterating through the hashtable with the helper function
  if ((fp = fopen(filename, "w")) != NULL) {
    hashtable_iterate(index->ht, fp, hashtable_item_save);
    fclose(fp);
  }
  else {
    fprintf(stderr, "%s could not be written into\n", filename);
    exit(0);
  }
}

/* helper function for hashtable_iterate */
static void hashtable_item_save(void* arg, const char* key, void* item) {
  FILE *fp = arg;
  // print the key
  fprintf(fp, "%s ", key);
  // on the same line, begin iterating through the counterset with the helper function
  counters_iterate(item, arg, counters_item_save);
  // move to next line
  fprintf(fp, "\n");
}

/* helper function for counters_iterate */
static void counters_item_save(void* arg, const int key, const int count) {
  FILE *fp = arg;
  // print in the format docID count docID count
  fprintf(fp, "%d %d ", key, count);
}

/**************** index_load() ****************/
/* loads the index from another index-produced file */
index_t* index_load(char *filename) {
  FILE *fp;
  if ((fp = fopen(filename, "r")) != NULL) {
    // initialize index to the exact size of lines in the file
    int size = file_numLines(fp);
    index_t *index = index_new(size);
    if (index == NULL) { // check that the index is allocated properly
      fprintf(stderr, "error allocating to index\n");
      return NULL;
    }
    char* word;
    int ID;
    int count;
    // begin looping through the words, so the loop increments line by line
    while ((word = file_readWord(fp)) != NULL) {
      // begin scanning all the docID count pairs until none are left, this is based on the assumption that index_save works
      while (fscanf(fp, "%d %d ", &ID, &count) == 2) {
        // set the value of the index
        index_set(index, word, ID, count);
      }
      free(word);
    }
    fclose(fp);
    return index;
  }
  fprintf(stderr, "File %s cannot be accessed\n", filename);
  return NULL;
}

/**************** index_page() ****************/
/* scans a webpage document to add its words to the index */
void index_page(index_t *index, webpage_t *page,  char* pageDirectory, int ID) {
  // loop through words in page
      int pos = 0;
      char* result;
      while ((result = webpage_getNextWord(page, &pos)) != NULL) {
        // word should be "significant"
        if (strlen(result) >= 3) {
          result = NormalizeWord(result);
          index_insert(index, result, ID);
          free(result);
        }
        else {
          free(result);
        }
      }
}

/**************** index_build() ****************/
/* builds an in-memory index from webpage files it finds in the pageDirectory */
index_t* index_build(char* pageDirectory) {
  // Assume there won't be more than 4 docID digits
  int len = strlen(pageDirectory) + 5;
  char *filename = malloc(sizeof(char) * len);
  // convert id to str and concat to pageDirectory. Assume first docID starts at 1
  int ID = 1;
  sprintf(filename, "%s/%d", pageDirectory, ID);
  FILE *fp; 
  // Create a new index
  index_t* index = index_new(ht_size);
  if (index != NULL) {
    while ((fp = fopen(filename, "r")) != NULL) { // will loop through files in the page directory, will update filename
      webpage_t *page;
      if ((page = pagedir_load(ID, pageDirectory)) != NULL) { // load crawler files into webpage data structures
        index_page(index, page, pageDirectory, ID);
        webpage_delete(page);
      }
      else {
        fprintf(stderr, "error loading page\n");
        free(filename);
        exit(1);
      } 
      fclose(fp);
      ID++;
      sprintf(filename, "%s/%d", pageDirectory, ID);
    }
    free(filename);
    return index;
  }
  else {
    fprintf(stderr, "error initializing index\n");
    free(filename);
    exit(1);
  }
}

/**************** index_delete() ****************/
/* Deletes the index and frees memory */
void index_delete(index_t *index) {
  // makes sure index is not null before freeing
  if (index != NULL) {
    hashtable_delete(index->ht, void_counters_delete);
    free(index);
  }
}

// Need this helper function because hashtable_delete expects a void type parameter
static void void_counters_delete(void *item) {
  counters_delete(item);
}
