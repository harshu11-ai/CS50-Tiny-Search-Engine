/*
    index.h
    Header file for index.c
    Author: Harshith Yallampalli
*/

#ifndef __INDEX_H
#define __INDEX_H

#include <stdbool.h>
#include "counters.h"

typedef struct index_t index_t;

/**************** index_new ****************/
/* Create a new (empty) index.
 * Caller is responsible for later calling index_delete.
 * We return: pointer to the new index, or NULL on error.
 */
index_t* index_new(void);

/**************** index_add ****************/
/* Add one occurrence of word in document docID to the index.
 * If word is new, it is inserted with count 1 for this docID.
 * If word exists, the count for this docID is incremented.
 * Caller provides: index, word (normalized), docID (>= 0).
 */
void index_add(index_t* index, const char* word, int docID);

/**************** index_find ****************/
/* Find the counters for a given word in the index.
 * Caller provides: index, word .
 * We return: pointer to counters_t for that word, or NULL if not found.
 * Returned pointer is owned by the index; caller must not delete it.
 */
counters_t* index_find(index_t* index, const char* word);

/**************** index_load ****************/
/* Load an index from a file. File format: one line per word,
 * line = word docID count [docID count]...
 * Caller provides: path/name of the index file.
 * We return: new index, or NULL on error. Caller must call index_delete.
 */
index_t* index_load(const char* indexFilePath);

/**************** index_save ****************/
/* Save the index to a file. One line per word: word docID count [docID count]...
 * Caller provides: index, path/name of the output file.
 * We return: true if successful, false otherwise.
 */
bool index_save(index_t* index, const char* indexFilePath);

/**************** index_delete ****************/
/* Delete the index and free all memory.
 * Caller provides: index (we ignore NULL).
 * We return: true if deleted, false if index was NULL.
 */
bool index_delete(index_t* index);

#endif
