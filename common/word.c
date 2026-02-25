/*
    word.c
    Implementation of word.h
    Author: Harshith Yallampalli
*/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"

/**************** normalizeWord ****************/
char* normalizeWord(const char* word)
{
    if (word == NULL) {
        return NULL;
    }
    char* normalized = malloc(strlen(word) + 1);
    if (normalized == NULL) {
        return NULL;
    }
    strcpy(normalized, word);
    for (size_t i = 0; normalized[i] != '\0'; i++) {
        normalized[i] = tolower((unsigned char) normalized[i]);
    }
    return normalized;
}