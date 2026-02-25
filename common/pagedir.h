/*
    pagedir.h
    Header file for pagedir.c
    Author: Harshith Yallampalli
*/

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "webpage.h"

/****************pagedir_init ****************/
/* Initialize the pagedir by creating the directory if it doesn't exist */
/* Caller provides: */
/*   pageDirectory: the directory to initialize */
/* We return: */
/*   true if successful, false otherwise */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_isCrawler ****************/
/* Check if pageDirectory is a crawler-produced directory (has .crawler marker).
 * Caller provides: pageDirectory.
 * We return: true if .crawler exists, false otherwise.
 */
bool pagedir_isCrawler(const char* pageDirectory);

/****************pagedir_save ****************/
/* Save the webpage to the pagedir */
/* Caller provides: */
/*   page: the webpage to save */
/*   pageDirectory: the directory to save the webpage to */
/*   docID: the document ID of the webpage */
/* We return:*/
/*  Print of the URL  */
/*  Print of the depth */
/*  Print of the contents of the webpage */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_load ****************/
/* Load a webpage from pageDirectory/docID file (crawler output format).
 * Caller provides: pageDirectory, docID.
 * We return: new webpage_t (caller must webpage_delete), or NULL on error.
 */
webpage_t* pagedir_load(const char* pageDirectory, const int docID);

#endif // __PAGEDIR_H
