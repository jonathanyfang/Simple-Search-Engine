/*
* index.h - 'CS50' index module
* 
* contains all the functions for the index data structure
* can initialize, insert into, set counts for, save, load, and delete indexes
* 
* Jonathan Fang
* CS50 Fall 2022
*/

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

/* creates a new struct index_t */
typedef struct index index_t;

/* creates a new, empty index */
index_t *index_new(int ht_size);

/* given an index, a word, and a docID, inserts the word into the index */
bool index_insert(index_t *index, const char* word, const int docID);

/* given an index, a word, a docID, and a count, 
uses counters_set to set the count of a counterset in the index*/
bool index_set(index_t *index, const char* word, const int docID, const int count);

/* saves the index to a file */
void index_save(index_t *index, const char* filename);

/* loads the index from another index-produced file */
index_t* index_load(char *filename);

/* Deletes the index and frees memory */
void index_delete(index_t *index);

#endif