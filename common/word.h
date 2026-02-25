/*
    word.h
    Header file for word.c
    Author: Harshith Yallampalli
*/

#ifndef __WORD_H
#define __WORD_H

#include <stdbool.h>

/**************** normalizeWord ****************/
/* Normalize a word: copy with all characters converted to lowercase.
 * Caller provides: word to normalize.
 * We return: new string (caller must free), or NULL on error or if word is NULL.
 */
char* normalizeWord(const char* word);

#endif