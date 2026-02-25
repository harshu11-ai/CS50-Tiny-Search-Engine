/*
    index.c
    Implementation of index.h
    Author: Harshith Yallampalli
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "index.h"
#include "hashtable.h"
#include "counters.h"
#include "file.h"

struct index_t {
    hashtable_t* ht;
};

/**************** count_print_helper ****************/
/* Helper for index_save: print one " docID count" pair to the file. */
static void count_print_helper(void* arg, const int key, const int count)
{
    FILE* fp = arg;
    if (fp != NULL) {
        fprintf(fp, " %d %d", key, count);
    }
}

/**************** word_save_helper ****************/
/* Helper for index_save: print one word and its docID/count pairs (one line). */
static void word_save_helper(void* arg, const char* key, void* item)
{
    FILE* fp = arg;
    counters_t* ctrs = item;
    if (fp == NULL || key == NULL || ctrs == NULL) {
        return;
    }
    fprintf(fp, "%s", key);
    counters_iterate(ctrs, fp, count_print_helper);
    fprintf(fp, "\n");
}

/**************** counters_delete_helper ****************/
/* Helper for index_delete: delete one counters_t item. */
static void counters_delete_helper(void* item)
{
    if (item != NULL) {
        counters_delete((counters_t*)item);
    }
}

/**************** index_new ****************/
index_t* index_new(void)
{
    index_t* index = malloc(sizeof(index_t));
    if (index == NULL) {
        return NULL;
    }
    index->ht = hashtable_new(900);
    if (index->ht == NULL) {
        free(index);
        return NULL;
    }
    return index;
}

/**************** index_add ****************/
void index_add(index_t* index, const char* word, int docID)
{
    if (index == NULL || word == NULL || docID < 0) {
        return;
    }
    counters_t* ctrs = hashtable_find(index->ht, word);
    if (ctrs == NULL) {
        ctrs = counters_new();
        if (ctrs == NULL) {
            return;
        }
        counters_add(ctrs, docID);
        if (!hashtable_insert(index->ht, word, ctrs)) {
            counters_delete(ctrs);
        }
    } else {
        counters_add(ctrs, docID);
    }
}

/**************** index_find ****************/
counters_t* index_find(index_t* index, const char* word)
{
    if (index == NULL || word == NULL) {
        return NULL;
    }
    return hashtable_find(index->ht, word);
}

/**************** index_load ****************/
index_t* index_load(const char* indexFilePath)
{
    if (indexFilePath == NULL) {
        return NULL;
    }
    index_t* index = index_new();
    if (index == NULL) {
        return NULL;
    }
    FILE* fp = fopen(indexFilePath, "r");
    if (fp == NULL) {
        index_delete(index);
        return NULL;
    }
    char* word;
    while ((word = file_readWord(fp)) != NULL) {
        if (word[0] == '\0') {
            free(word);
            continue;
        }
        int docID;
        int count;
        while (fscanf(fp, "%d %d", &docID, &count) == 2) {
            counters_t* ctrs = hashtable_find(index->ht, word);
            if (ctrs == NULL) {
                ctrs = counters_new();
                if (ctrs == NULL) {
                    free(word);
                    fclose(fp);
                    index_delete(index);
                    return NULL;
                }
                if (!counters_set(ctrs, docID, count)) {
                    counters_delete(ctrs);
                    free(word);
                    fclose(fp);
                    index_delete(index);
                    return NULL;
                }
                if (!hashtable_insert(index->ht, word, ctrs)) {
                    counters_delete(ctrs);
                }
            } else {
                counters_set(ctrs, docID, count);
            }
        }
        free(word);
    }
    fclose(fp);
    return index;
}

/**************** index_save ****************/
bool index_save(index_t* index, const char* indexFilePath)
{
    if (index == NULL || indexFilePath == NULL) {
        return false;
    }
    FILE* fp = fopen(indexFilePath, "w");
    if (fp == NULL) {
        return false;
    }
    hashtable_iterate(index->ht, fp, word_save_helper);
    fclose(fp);
    return true;
}

/**************** index_delete ****************/
bool index_delete(index_t* index)
{
    if (index == NULL) {
        return false;
    }
    hashtable_delete(index->ht, counters_delete_helper);
    free(index);
    return true;
}
