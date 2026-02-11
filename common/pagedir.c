/*
    pagedir.c
    Implementation of pagedir.h
    Author: Harshith Yallampalli
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "pagedir.h"
#include "webpage.h"

/****************pagedir_init ****************/
bool pagedir_init(const char* pageDirectory) {
    if (pageDirectory == NULL) {
        return false;
    }

    const char* marker = "/.crawler";
    size_t pathLen = strlen(pageDirectory) + strlen(marker) + 1;
    char* filepath = malloc(pathLen);
    if (filepath == NULL) {
        return false;
    }

    sprintf(filepath, "%s%s", pageDirectory, marker);

    FILE* fp = fopen(filepath, "w");
    free(filepath);

    if (fp == NULL) {
        return false;
    }

    fclose(fp);
    return true;
}

/****************pagedir_save ****************/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
    if (page == NULL || pageDirectory == NULL) {
        return;
    }

    size_t pathLen = strlen(pageDirectory) + 1 + 20;
    char* filepath = malloc(pathLen);
    if (filepath == NULL) {
        return;
    }

    sprintf(filepath, "%s/%d", pageDirectory, docID);

    FILE* fp = fopen(filepath, "w");
    free(filepath);

    if (fp == NULL) {
        return;
    }

    const char* url   = webpage_getURL(page);
    int depth = webpage_getDepth(page);
    const char* html  = webpage_getHTML(page);

    if (url != NULL) {
        fprintf(fp, "%s\n", url);
    } else {
        fprintf(fp, "\n");
    }

    fprintf(fp, "%d\n", depth);

    if (html != NULL) {
        fprintf(fp, "%s\n", html);
    } else {
        fprintf(fp, "\n");
    }

    fclose(fp);
}