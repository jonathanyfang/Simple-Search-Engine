/*
* word.c 
* 
* see word.h for more info
* 
* Johnny Fang, October 20, 2022
* CS50, Fall 2022
*/

#include <ctype.h>
#include <string.h>
#include "word.h"

// changes memory directly, so no need to return a char*
char *NormalizeWord(char* word) {
    int len = strlen(word);
    if (word == NULL) {
        return word;
    }
    // loops over every char in word and uses tolower to normalize the word
    for(int i = 0; i < len; i++){
        word[i] = tolower(word[i]);
    }
    return word;
}