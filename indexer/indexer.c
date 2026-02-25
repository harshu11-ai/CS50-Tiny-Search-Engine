/*
    indexer.c
    Scans a crawler output directory and builds an index file.
    Author: Harshith Yallampalli
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "index.h"
#include "word.h"
#include "pagedir.h"
#include "webpage.h"

/**************** indexPage ****************/
/* function to index a webpage */
/* Caller provides: webpage, docID, and index */
/* We return: void */
static void indexPage(webpage_t* webpage, int docID, index_t* index)
{
    int pos = 0;
    char* word;
    while ((word = webpage_getNextWord(webpage, &pos)) != NULL) {
        if (strlen(word) < 3) {
            free(word);
            continue;
        }
        char* normalized = normalizeWord(word);
        free(word);
        if (normalized == NULL) {
            continue;
        }
        index_add(index, normalized, docID);
        free(normalized);
    }
}

/**************** indexBuild ****************/
/* function to build the index */
/* Caller provides: page directory and index filename */
/* We return: void */
static void indexBuild(const char* pageDirectory, const char* indexFilename)
{
    index_t* index = index_new();
    if (index == NULL) {
        fprintf(stderr, "Error: Failed to create index\n");
        return;
    }
    int docID = 1;
    webpage_t* webpage;
    while ((webpage = pagedir_load(pageDirectory, docID)) != NULL) {
        indexPage(webpage, docID, index);
        webpage_delete(webpage);
        docID++;
    }
    index_save(index, indexFilename);
    index_delete(index);
}

/**************** main ****************/
/* function to main */
/* Caller provides: argc and argv */
/* We return: 0 if successful, 1 if error */
int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s pageDirectory indexFilename\n", argv[0]);
        return 1;
    }
    const char* pageDirectory = argv[1];
    const char* indexFilename = argv[2];

    if (!pagedir_isCrawler(pageDirectory)) {
        fprintf(stderr, "Error: '%s' is not a crawler directory\n", pageDirectory);
        return 1;
    }

    indexBuild(pageDirectory, indexFilename);
    return 0;
}
