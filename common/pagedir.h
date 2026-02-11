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

#endif // __PAGEDIR_H
