/* 
Harshith Yallampalli
CS50 Winter 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <bag.h>
#include <hashtable.h>
#include <webpage.h>
#include "pagedir.h"
#include "crawler.h"

#define MAX_DEPTH 10

/*
 * Page scan function
 * Caller provides:
 *   page: the webpage to scan
 *   bag: the bag to insert the new pages into
 *   ht: the hashtable to insert the new pages into
 */
static void pageScan(webpage_t* page, bag_t* bag, hashtable_t* ht)
{
    // scan the webpage
    int pos = 0;
    char* url;
    int depth = webpage_getDepth(page);

    // scan the webpage
    while ((url = webpage_getNextURL(page, &pos)) != NULL) {
        // normalize the URL
        char* normalized = normalizeURL(url);
        free(url);
        // check if the URL is internal
        if (normalized != NULL && isInternalURL(normalized)) {
            // insert the URL into the hashtable
            if (hashtable_insert(ht, normalized, (void*)(depth + 1))) {
                webpage_t* newpage = webpage_new(normalized, depth + 1, NULL);
                if (newpage != NULL) {
                    // insert the new page into the bag
                    bag_insert(bag, newpage);
                } else {
                    free(normalized);
                }
            } else {
                // URL already seen, free the normalized URL
                free(normalized);
            }
        } else {
            if (normalized != NULL) {
                free(normalized);
            }
        }
    }
}

/*
 * Crawler function
 * Caller provides:
 *   seedUrl: the seed URL to crawl from
 *   pageDirectory: the directory to save the webpages to
 *   maxDepth: the maximum depth to crawl to
 */
static void crawl(char *seedUrl, char *pageDirectory, int maxDepth)
{
    // initialize the bag and hashtable
    bag_t* bag = bag_new();
    hashtable_t* hashtable = hashtable_new(200);
    webpage_t* seed = webpage_new(seedUrl, 0, NULL);
    int docID = 1;
    bag_insert(bag, seed);
    hashtable_insert(hashtable, seedUrl, (void*)0);

    // crawl the web from the seed URL
    webpage_t* page;
    while ((page = bag_extract(bag)) != NULL) {
        int depth = webpage_getDepth(page);
        if (depth <= maxDepth && webpage_fetch(page)) {
            // scan the webpage
            pageScan(page, bag, hashtable);
            // save the webpage
            pagedir_save(page, pageDirectory, docID);
            docID++;
        }
        webpage_delete(page);
    }

    // delete the bag and hashtable
    bag_delete(bag, NULL);
    hashtable_delete(hashtable, NULL);
}

/*
 * Parse arguments function
 * Caller provides:
 *   argc: the number of arguments
 *   argv: the arguments
 *   seedUrl: the seed URL to crawl from
 *   pageDirectory: the directory to save the webpages to
 *   maxDepth: the maximum depth to crawl to
 */
static void parseArgs(int argc, char *argv[], char **seedUrl, char **pageDirectory, int *maxDepth)
{
    if (argc != 4) {
        fprintf(stderr, "Usage: %s seedURL pageDirectory maxDepth\n", argv[0]);
        exit(1);
    }

    // seedURL: normalize and ensure it's an internal URL
    *seedUrl = argv[1];
    char *normalized = normalizeURL(*seedUrl);
    if (normalized == NULL) {
        fprintf(stderr, "Error: invalid seedURL\n");
        exit(1);
    }
    if (!isInternalURL(normalized)) {
        fprintf(stderr, "Error: seedURL must be an internal URL\n");
        free(normalized);
        exit(1);
    }
    *seedUrl = normalized;

    // pageDirectory: initialize with pagedir_init
    *pageDirectory = argv[2];
    if (!pagedir_init(*pageDirectory)) {
        fprintf(stderr, "Error: cannot initialize pageDirectory '%s'\n", *pageDirectory);
        exit(1);
    }

    // maxDepth: ensure it is greater than 0
    *maxDepth = atoi(argv[3]);
    if (*maxDepth < 0 || *maxDepth > MAX_DEPTH) {
        fprintf(stderr, "Error: maxDepth must be greater than 0 and less than %d\n", MAX_DEPTH);
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    char *seedUrl, *pageDirectory;
    int maxDepth;
    parseArgs(argc, argv, &seedUrl, &pageDirectory, &maxDepth);
    crawl(seedUrl, pageDirectory, maxDepth);
    return 0;
} 