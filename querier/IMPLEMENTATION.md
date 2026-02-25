## Implementation Spec

In this document we reference the Requirements Specification and Design Specification and focus on the implementation-specific decisions. 

## Data Structures 
We use 1 main data structure: a counter to keep track of each iteration of a word present based on the query provided. The counters are also used to interpret if and/or operators are used. 

## Control Flow

The Querier is implemented in one file called the querier.c

### main
The main function parses pageDirectory and indexFilename that were created in the prior lab assignments. On wrong argument count, prints usage to stderr and exits with non-zero status.

Usage: `querier pageDirectory indexFilename`

### queryloop
queryloop runs an interactive loop. For each line of input it: reads the query string, tokenizes it into words, prints the normalized query, checks that and/or are not first, last, or adjacent, and if valid calls answerQuery to compute and print document scores. Pseudocode:

```text
while (true)
    print "Query? "
    if fgets on stdin returns NULL, break
    strip trailing newline from query
    tokens = tokenize(query)
    if tokens is NULL or first token is NULL, continue
    print the tokens as the normalized query
    check that "and"/"or" are not first, last, or adjacent
    if query is valid, call answerQuery(index, tokens, pageDirectory)
    free tokens array
```

### tokenize
tokenize splits the input query into whitespace-separated words and returns an array of pointers into the original buffer. Pseudocode:

```text
allocate an array of char* of fixed size
use strtok over space/tab/newline to find each word
terminate the array with a NULL pointer
return the array (caller later frees the array)
```

### matchQuery
matchQuery interprets the token list as an OR of AND‑sequences. It looks up each word in the index that is provided and combines their counters with intersection for and and union for or(sum of counts), and returns a counters set of scores per document. Pseudocode:

```text
create empty counters orResult
set andResult to NULL
for each token in words
    if token is "or"
        if andResult is not NULL
            add all doc counts from andResult into orResult (union)
            delete andResult and set to NULL
        continue
    if token is "and", continue
    wordCounters = index_find(index, token)
    if wordCounters is NULL
        if andResult is not NULL, delete it and set to NULL
        continue
    if andResult is NULL
        create new counters andResult
        copy all entries from wordCounters into andResult (union)
    else
        for each doc in andResult
            replace its count with min(current, count in wordCounters)
after loop, if andResult not NULL
    union its counts into orResult and delete it
return orResult
```

### answerQuery
answerQuery calls matchQuery, then repeatedly finds the highest‑scoring document in the result counters. It then loads that page with pagedir_load and prints its score, document ID, and URL. After printing a document it sets its score to zero and continues until no positive scores remain, then frees all allocated structures. Pseudocode:

```text
matches = matchQuery(index, tokens)
if matches is NULL
    print "No documents match." and return
loop
    find docID with highest score in matches
    if best score <= 0, break
    page = pagedir_load(pageDirectory, docID)
    url = webpage_getURL(page) if page not NULL
    print "score <score> doc <docID>: <url>"
    delete page if not NULL
    set this docID's score in matches to 0
print a separator line
delete matches
```

## Function Prototypes
Detailed descriptions of each function are provided above and are not repeated here.
```text
int main(int argc, char* argv[]);
static void queryloop(index_t* index, const char* pageDirectory);
static char** tokenize(char* query);
static counters_t* matchQuery(index_t* index, char** words);
static void answerQuery(index_t* index, char** result, const char* pageDirectory);
```

## Error handling and recovery
All problems are reviewed accordingly and produce a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the mem_assert functions, which result in a message printed to stderr and a non-zero exit status. We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the mem_assert functions), e.g., if a function receives bad parameters.

