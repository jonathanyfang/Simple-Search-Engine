/*
* indextest.c
* 
* ./indextest oldIndexFilename newIndexFilename
* pageDirectory should be a crawler-created output directory
* the index created should be written into indexFilename
* 
* Jonathan Fang
* CS50, Fall 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../common/index.h"

/*
* return 0: no error
* return 1: incorrect number of arguments
* return 2: oldIndexFilename can't be read from
* return 3: newIndexFilename can't be written into
*/

// Uncomment out the below to print loaded sucessfully statement
// #define APPTEST 1

int main (const int argc, char *argv[]) {
    // verify proper number of arguments or return nonzero
    if (argc != 3) {
        fprintf(stderr, "Usage: ./indextest oldIndexFilename newIndexFilename\n");
        return(1);
    }
    char* oldIndexFilename = argv[1];
    char* newIndexFilename = argv[2];
    FILE *fp;
    // check that oldIndexFilename can be read
    if ((fp = fopen(oldIndexFilename, "r")) == NULL) {
        fprintf(stderr, "File %s cannot be read from\n", oldIndexFilename);
        return(2);
    }
    // load the index based on oldIndexFilename. index_load takes care of allocating errors
    index_t *index = index_load(oldIndexFilename);
    #ifdef APPTEST
    fprintf(stderr, "successfully loaded\n");
    #endif
    // done with oldIndexFilename, close fp
    fclose(fp);
    // check that newIndexFilename can be written into
    if ((fp = fopen(newIndexFilename, "w")) == NULL) {
        index_delete(index);
        fprintf(stderr, "File %s cannot be written into\n", newIndexFilename);
        return(3);
    }
    fclose(fp);
    index_save(index, newIndexFilename);
    index_delete(index); // clean up
    return 0;
}
