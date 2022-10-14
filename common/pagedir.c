/*
* pagedir.c
* 
* see pagedir.h for more info
* 
* Jonathan Fang, October 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

// checks if the directory is writable/exists
bool pagedir_init(char *pageDirectory) {
    char* str = ".crawler";
    int len = strlen(pageDirectory) + strlen(str) + 2;
    char *filename = malloc(sizeof(char) * len);
    // create a char* in the format directoryname/.crawler
    sprintf(filename, "%s/%s", pageDirectory, str);
    FILE *fp; 
    // test if the file can be written into
	if ((fp = fopen(filename, "w")) == NULL) {
        // directory doesn't exist
        free(filename);
        fprintf(stderr, "%s cannot be written, directory DNE or is not writable", filename);
        return false;
	}
	else { 
        fclose(fp);
		free(filename);
        return true;
	}
}

// outputs a page to the appropriate file
void pagedir_save(webpage_t *page, int id, char *pageDirectory) {
    // find the number of digits in id
    int digits = 0;
    int temp = id;
    while (temp != 0) {
        temp /= 10;
        digits++;
    }
    int len = strlen(pageDirectory) + digits + 2;
    char *filename = malloc(sizeof(char) * len);
    // convert id to str and concat to pageDirectory
    sprintf(filename, "%s/%d", pageDirectory, id);
    FILE *fp; 
	if ((fp = fopen(filename, "w")) != NULL) {
        // write url, dpeth, and html 
		fprintf(fp, "%s\n", webpage_getURL(page));
		fprintf(fp, "%d\n", webpage_getDepth(page));
		fprintf(fp, "%s\n", webpage_getHTML(page));
        fclose(fp);
	}
	else {
		fprintf(stderr, "%s cannot be written\n", filename);
	}
    free(filename);
}