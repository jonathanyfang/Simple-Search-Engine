/*
* word.h 
* 
* implements a function NormalizeWord to make all letters in a str lowercase
* 
* Johnny Fang, October 20, 2022
* CS50, Fall 2022
*/

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <stdbool.h>

// Sends all the chars in a str to lowercase
char* NormalizeWord(char* word);

// Helper function to check if word is an operator "and" or "or"
bool isOperator(char* word);

// Parses through and makes sure the string is only letters and spaces
// If so, tokenize the query string (char* line)
bool tokenizeString(char* line, char* tokenArr[], int* len);

// Validates the query:
// Neither the first or last word may be an operator, and two operators may not be adjacent. 
// If valid, return true; otherwise print a suitable error message and return false.
bool validateQuery(char* tokenArr[], int len);


#endif