/*
    crawler.h
    Header file for crawler.c
    Author: Harshith Yallampalli
*/

#ifndef __CRAWLER_H
#define __CRAWLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"

/****************crawler ****************/
/* Crawl the web from the seed URL */
/* Caller provides: */
/*   seedUrl: the URL to crawl from */
/*   pageDirectory: the directory to save the webpages to */
/*   maxDepth: the maximum depth to crawl to */
/* We return: */
/*   true if successful, false otherwise */
static void crawl(char *seedUrl, char *pageDirectory, int maxDepth);

/****************parseArgs ****************/
/* Parse the arguments */
/* Caller provides: */
/*   argc: the number of arguments */
/*   argv: the arguments */
/* We return: */
/*   true if successful, false otherwise */
static void parseArgs(int argc, char *argv[], char **seedUrl, char **pageDirectory, int *maxDepth);


/****************pageScan ****************/
/* Scan the webpage */
/* Caller provides: */
/*   page: the webpage to scan */
/*   bag: the bag to save the webpages to */
/*   ht: the hashtable to save the webpages to */
/* We return: */
/*   true if successful, false otherwise */
static void pageScan(webpage_t* page, bag_t* bag, hashtable_t* ht);

#endif // __CRAWLER_H