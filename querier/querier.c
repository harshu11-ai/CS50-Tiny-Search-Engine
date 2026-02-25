/*
 * querier.c
 *
 *      Author: harshu11
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h>
#include "index.h"
#include "counters.h"
#include "pagedir.h"
#include "webpage.h"

/* tokenize: Returns a null-terminated array of pointers into query.
 * Caller frees the array but not the individual tokens.
 */
static char** tokenize(char *query) {
    char **tokens = malloc(sizeof(char *) * 101);
    if (tokens == NULL) {
        return NULL;
    }
    int i = 0;
    /* Used strtok to tokenize the query into words with the help of ChatGPT*/
    char *token = strtok(query, " \t\n");
    while (token != NULL && i < 100) {
        tokens[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    tokens[i] = NULL;
    return tokens;
}

/* minimal global state and callbacks for counters operations */

static counters_t *g_intersect_dest = NULL;
static counters_t *g_intersect_other = NULL;
static counters_t *g_union_dest = NULL;

static void intersect_helper(void *arg, const int key, const int count) {
    (void)arg;
    int otherCount = counters_get(g_intersect_other, key);
    if (otherCount == 0) {
        counters_set(g_intersect_dest, key, 0);
    } else if (otherCount < count) {
        counters_set(g_intersect_dest, key, otherCount);
    }
}

static void union_helper(void *arg, const int key, const int count) {
    (void)arg;
    int old = counters_get(g_union_dest, key);
    counters_set(g_union_dest, key, old + count);
}

/* findBest: helper to find highest-scoring doc in counters */
static int g_bestDoc = 0;
static int g_bestScore = 0;

static void best_helper(void *arg, const int key, const int count) {
    (void)arg;
    if (count > g_bestScore) {
        g_bestScore = count;
        g_bestDoc = key;
    }
}

/* matchQuery: interpret words as OR of AND-sequences and return scores */
static counters_t*
matchQuery(index_t *index, char **words) {
    counters_t *orResult = counters_new();
    if (orResult == NULL) {
        return NULL;
    }

    counters_t *andResult = NULL;

    for (int i = 0; words[i] != NULL; i++) {
        if (words[i] == "or") {
            if (andResult != NULL) {
                g_union_dest = orResult;
                counters_iterate(andResult, NULL, union_helper);
                counters_delete(andResult);
                andResult = NULL;
            }
            continue;
        }

        if (words[i] == "and") {
            continue;
        }

        counters_t *wordCounters = index_find(index, words[i]);
        if (wordCounters == NULL) {
            if (andResult != NULL) {
                counters_delete(andResult);
                andResult = NULL;
            }
            continue;
        }

        if (andResult == NULL) {
            andResult = counters_new();
            if (andResult == NULL) {
                counters_delete(orResult);
                return NULL;
            }
            /* copy wordCounters into andResult using union_helper */
            g_union_dest = andResult;
            counters_iterate(wordCounters, NULL, union_helper);
        } else {
            /* intersect andResult with wordCounters: AND = min counts */
            g_intersect_dest = andResult;
            g_intersect_other = wordCounters;
            counters_iterate(andResult, NULL, intersect_helper);
        }
    }

    if (andResult != NULL) {
        g_union_dest = orResult;
        counters_iterate(andResult, NULL, union_helper);
        counters_delete(andResult);
    }

    return orResult;
}

void answerQuery(index_t *index, char **result, const char *pageDirectory) {
    counters_t *matches = matchQuery(index, result);
    if (matches == NULL) {
        printf("No documents match.\n");
        return;
    }

    while (true) {
        g_bestDoc = 0;
        g_bestScore = 0;
        counters_iterate(matches, NULL, best_helper);

        if (g_bestScore <= 0) {
            break;
        }

        webpage_t *page = pagedir_load(pageDirectory, g_bestDoc);
        char *url = NULL;
        if (page != NULL) {
            url = webpage_getURL(page);
        }

        printf("score %d doc %d: %s\n", g_bestScore, g_bestDoc, url);

        if (page != NULL) {
            webpage_delete(page);
        }

        counters_set(matches, g_bestDoc, 0);
    }

    printf("---------------------------------\n");

    counters_delete(matches);
}

void queryloop(index_t *index, const char *pageDirectory)
{
    char query[2048];
    while (true) {
        printf("Query? ");
        if (fgets(query, sizeof(query), stdin) == NULL) {
            break;
        }
        query[strcspn(query, "\n")] = '\0';

        char **result = tokenize(query);
        if (result == NULL) {
            continue;
        }
        if (result[0] == NULL) {
            free(result);
            continue;
        }

        printf("Query:");
        for (int i = 0; result[i] != NULL; i++) {
            printf(" %s", result[i]);
        }
        printf("\n");

        bool valid = true;
        if (strcmp(result[0], "and") == 0 || strcmp(result[0], "or") == 0) {
            printf("Error: %s cannot be first\n", result[0]);
            valid = false;
        }
        for (int i = 0; valid && result[i] != NULL; i++) {
            if (strcmp(result[i], "and") == 0 || strcmp(result[i], "or") == 0) {
                if (result[i + 1] == NULL) {
                    printf("Error: %s cannot be last\n", result[i]);
                    valid = false;
                } else if (strcmp(result[i + 1], "and") == 0 || strcmp(result[i + 1], "or") == 0) {
                    printf("Error: %s and %s cannot be adjacent\n", result[i], result[i + 1]);
                    valid = false;
                }
            }
        }
        if (valid) {
            answerQuery(index, result, pageDirectory);
        }
        free(result);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s pageDirectory indexFilename\n", argv[0]);
        exit(1);
    }
    const char *pageDirectory = argv[1];
    const char *indexFilename = argv[2];

    index_t *index = index_load(indexFilename);
    if (index == NULL)
    {
        fprintf(stderr, "Error: cannot load index from %s\n", indexFilename);
        exit(1);
    }

    if (!pagedir_isCrawler(pageDirectory))
    {
        fprintf(stderr, "Error: '%s' is not a crawler directory\n", pageDirectory);
        exit(1);
    }

    queryloop(index, pageDirectory);
    index_delete(index);
    return 0;
}