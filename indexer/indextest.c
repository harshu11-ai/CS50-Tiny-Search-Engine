/*
 * indextest.c
 * Loads an index file into memory and saves it to a new file.
 * Serves as a unit test for the index module (index_load and index_save).
 * Author: Harshith Yallampalli
 */

#include <stdio.h>
#include <stdlib.h>

#include "index.h"

int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s oldIndexFilename newIndexFilename\n", argv[0]);
        return 1;
    }

    const char* oldIndexFile = argv[1];
    const char* newIndexFile = argv[2];

    index_t* index = index_load(oldIndexFile);
    if (index == NULL) {
        fprintf(stderr, "Error: could not load index from '%s'\n", oldIndexFile);
        return 1;
    }

    if (!index_save(index, newIndexFile)) {
        fprintf(stderr, "Error: could not save index to '%s'\n", newIndexFile);
        index_delete(index);
        return 1;
    }

    index_delete(index);
    return 0;
}
