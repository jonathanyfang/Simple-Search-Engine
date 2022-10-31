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

// Helper function to check if word is an operator "and" or "or"
bool isOperator(char* word) {
    // strcmp the input to "and" and "or"
    if (strcmp(word, "and") != 0 && strcmp(word, "or") != 0) {
        return false;
    }
    return true;
}

// Parses through and makes sure the string is only letters and spaces
// If so, tokenize the query string (char* line)
bool tokenizeString(char* line, char* tokenArr[], int* len) {
    char* word = strtok(line, " ");
    // Empty query 
    if (word == NULL) {
        return false;
    }
    int index = 0;
    // check to make sure the word is alphabetical
    while(word != NULL) {
        for(int i = 0; i < strlen(word); i++) {
            if(!isalpha(word[i])) { 
                fprintf(stderr, "Error: bad character '%c' in query\n", word[i]);
                return false;
            }
        }
        // word is alphabetical
        tokenArr[index] = NormalizeWord(word);
        word = strtok (NULL, " "); // move to next token
        index++;
    }
    *len = index;
    return true;
}

// Validates the query:
// Neither the first or last word may be an operator, and two operators may not be adjacent. 
// If valid, return true; otherwise print a suitable error message and return false.
bool validateQuery(char* tokenArr[], int len) {
    // Already checked for empty queries
    // Check for first and last word
    if (isOperator(tokenArr[0])) {
        fprintf(stderr, "Error: '%s' cannot be first\n", tokenArr[0]);
        return false;
    }
    else if (isOperator(tokenArr[len-1])) {
        fprintf(stderr, "Error: '%s' cannot be last\n", tokenArr[len-1]);
        return false;
    }
    // Check for adjacent operators
    for (int i = 0; i < len - 1; i++) {
        if (isOperator(tokenArr[i]) && isOperator(tokenArr[i+1])) {
            fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", tokenArr[i], tokenArr[i+1]);
            return false;
        }
    }
    return true;
}