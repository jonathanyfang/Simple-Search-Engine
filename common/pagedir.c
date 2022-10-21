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
        fprintf(stderr, "%s cannot be written, directory DNE or is not writable\n", filename);
        free(filename);
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

// checks if the directory is a crawler output
bool pagedir_validate(char *pageDirectory) {
    char* str = ".crawler";
    int len = strlen(pageDirectory) + strlen(str) + 2;
    char *filename = malloc(sizeof(char) * len);
    // create a char* in the format directoryname/.crawler
    sprintf(filename, "%s/%s", pageDirectory, str);
    FILE *fp; 
    // test if the file can be read
	if ((fp = fopen(filename, "r")) != NULL) {
        fclose(fp);
	}
	else { // directory doesn't exist or cannot be read
        fclose(fp);
		fprintf(stderr, "%s is not a crawler output directory or cannot be read\n", pageDirectory);
        free(filename);
        return false;
	}
    free(filename);
    return true;
}

// loads a page from the crawler-produced directory
// creates and returns a webpage, it should later be deleted to free up any memory taken up
webpage_t* pagedir_load(int ID, const char* dirname) {
    // Generate the path
    int digits = 0;
    int temp = ID;
    while (temp != 0) {
        temp /= 10;
        digits++;
    }
    int len = strlen(dirname) + digits + 2;
    char *filename = malloc(sizeof(char) * len);
    // convert id to str and concat to pageDirectory
    sprintf(filename, "%s/%d", dirname, ID);
    FILE *fp; 
	if ((fp = fopen(filename, "r")) != NULL) {
        // first line is the url
        char* url = file_readLine(fp);
        // depth does not matter in this case, so we free the second line
        free(file_readLine(fp));
        // read the rest of the file to get the html
        char *html = file_readFile(fp);
        if(html != NULL) {
            webpage_t* page = webpage_new(url, 0, html);
            if (page == NULL) { // make sure page is allocated properly, if not clean up
                fclose(fp);
                free(filename);
                free(html);
                free(url);
                fprintf(stderr, "error allocating webpage\n");
                return NULL;
            }
            else {
                fclose(fp);
                free(filename);
                return page;
            }
        }
    }
    // If can't write into the filname, need to clean up memory
    fprintf(stderr, "%s cannot be written\n", filename);
    free(filename);
    return NULL;
}