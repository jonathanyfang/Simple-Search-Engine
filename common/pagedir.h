/*
* pagedir.c
* 
* contains some common functions, including initializing and verifying crawler directories
* also saves and loads pages to and from the crawler directories
* 
* Jonathan Fang, October 2022
*/

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include "../libcs50/webpage.h"

// checks if the directory is writable/exists
bool pagedir_init(char *pageDirectory);

// saves the webpage information into a file with a unique ID in the pageDirectory
void pagedir_save(webpage_t *page, int id, char *pageDirectory);

// checks if the directory is a crawler output
bool pagedir_validate(char *pageDirectory);

// loads a page from the crawler-produced directory
webpage_t* pagedir_load(int ID, const char* dirname);

#endif