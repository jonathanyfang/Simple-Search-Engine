/*
* indexer.c - CS50 'indexer'
* 
* usage: ./indexer pageDirectory indexFilename
* pageDirectory should be a crawler-created output directory
* the index created should be written into indexFilename
* 
* Jonathan Fang
* CS50, Fall 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"

void indexer(char *pageDirectory, char *indexFilename);
bool parseArgs(int argc, char* argv[]);


// Uncomment the line below to print success messages
// #define APPTEST 1

/*
* return 1: wrong number of args
* return 2: pageDirectory is not crawler-produced
* return 3: indexFilename cannot be written into
*/
int main (const int argc, char *argv[]) {
    // parseArgs will exit it if there are any errors
    #ifdef APPTEST
    printf("Main reached\n");
    #endif
    if (!parseArgs(argc, argv)) {
        exit(EXIT_FAILURE);
    }
    #ifdef APPTEST
    printf("args parsed\n");
    #endif
    // begin the indexer
    char* pageDirectory = argv[1];
    char* indexFilename = argv[2];
    indexer(pageDirectory, indexFilename);
    #ifdef APPTEST
    printf("Successfully indexed!\n");
    #endif
    return 0;
}   

bool parseArgs(int argc, char* argv[]) {
    char* pageDirectory = argv[1];
    char* indexFilename = argv[2];
    if (argc != 3) { // usage: ./indexer pageDirectory indexFilename
        fprintf(stderr, "Usage: ./indexer pageDirectory indexFilename\n");
        return false;
    }
    if (!pagedir_validate(pageDirectory)) { // Make sure the directory pageDirectory is crawler-produced and readable
        // no need to print anything, pagedir validate already does
        return false;
    }
    FILE *fp;
    if ((fp = fopen(indexFilename, "w")) == NULL) { // check that indexFilename is writable
        fprintf(stderr, "%s is not the pathname for a writable file\n", indexFilename);
        return false;
    }
    fclose(fp);
    return true;
}

void indexer(char *pageDirectory, char *indexFilename) {
    // initialize index to a random size
    #ifdef APPTEST
    printf("indexer reached!\n");
    #endif
    index_t* index = index_build(pageDirectory);
    #ifdef APPTEST
    printf("index built!\n");
    #endif
    index_save(index, indexFilename);
    #ifdef APPTEST
    printf("index saved\n");
    #endif
    index_delete(index);
}
