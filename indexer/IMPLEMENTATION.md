## Implementation Spec

In this document we reference the Requirements Specification and Design Specification and focus on the implementation-specific decisions. 

## Data Structures 
We use two data structures: a hashtable to store each word detected in a document as the key. The item for each key is a counter with the document ID(s) and the number of appearances of each word.

## Control Flow

The Indexer is implemented in one file called the indexer.c

### main
The main function parses two arguments (pageDirectory and indexFilename), then calls indexBuild. On wrong argument count, prints usage to stderr and exits with non-zero status.

Usage: `indexer pageDirectory indexFilename`

### indexBuild
indexBuild creates an in-memory index from webpage files in the pageDirectory, then saves it to indexFilename. It uses pagedir_load from common to load each document. Pseudocode:
```text
creates a new index using index_new
docID = 1
while (webpage = pagedir_load(pageDirectory, docID)) is not NULL
    calls indexPage(webpage, docID, index)
    webpage_delete(webpage)
    docID++
saves the index to indexFilename with index_save
index_delete(index)
```

### indexPage
indexPage scans a webpage and adds its words to the index. The caller (indexBuild) owns the webpage and deletes it after indexPage returns. Pseudocode:
```text
loops through each word of the webpage (webpage_getNextWord)
    if word length less than 3, skips the word and moves onto the next one
    normalizes the word with normalizeWord (word.c from common)
    index_add(index, normalized, docID)
    frees the word and normalized string
```

### indextest
indextest is a separate program that acts as a unit test for the index module. It loads an index file with index_load, then saves it to a new file with index_save. If the two files are identical (checked by diff in testing.sh), the index load/save logic is correct.

Usage: `indextest oldIndexFilename newIndexFilename`

Pseudocode for main:
```text
if argc != 3, print usage to stderr, return 1
index = index_load(oldIndexFilename)
if index is NULL, print error, return 1
if !index_save(index, newIndexFilename), print error, index_delete(index), return 1
index_delete(index), return 0
```

## Other Modules
### pagedir
The indexer uses pagedir_load from the common library to load a saved webpage from a crawler output file.

Pseudocode for pagedir_load():
```text
build path pageDirectory/docID and open file for reading
if file cannot be opened, return NULL
read first line (URL) with file_readLine
read second line (depth) with file_readLine, convert to int
read remainder of file (HTML) with file_readFile
create and return webpage_new(url, depth, html); caller must webpage_delete
return NULL on any error (and free any allocated strings)
```

### index
We create a reusable module index.c to implement an abstract index_t type that represents an index in memory, and support functions 

Pseudocode for index_new():
```text
initializes a new hashtable of size 200 to 900 
```

Pseudocode for index_delete():
```text
deletes provided hashtable 
frees memory
```

Pseudocode for index_save(): 
```text
create a new file within a directory
save the index file
```

Pseudocode for index_add():
```text
if word is already in the index hashtable
    increment the count for this docID in the word's counters
else
    create a new counters for this word, set count to 1 for this docID
    insert (word, counters) into the hashtable
```

Pseudocode for index_load():
```text
create a new index with index_new
open the index file for reading
for each line: read the word (e.g. with file_readWord), then loop with fscanf "%d %d" to get docID and count pairs
    for each (docID, count): add the word to the index with that docID and count (using hashtable and counters)
close the file
return the index (or NULL on error)
```

## word
Create a reusable module word.c to normalize words. 

Pseudocode for normalizeWord():
```text
allocates a copy of the word
converts each character to lower case (e.g. with tolower)
returns the normalized string (caller must free)
```

## Function Prototypes

### indexer
Detailed descriptions of each function are provided above and are not repeated here.
```text
int main(int argc, char* argv[]);
static void indexBuild(const char* pageDirectory, const char* indexFilename);
static void indexPage(webpage_t* webpage, int docID, index_t* index);
```

### index
Detailed descriptions of each function are provided above and are not repeated here.
```text
index_t* index_new(void);
void index_add(index_t* index, const char* word, int docID);
index_t* index_load(const char* indexFilePath);
bool index_save(index_t* index, const char* indexFilePath);
bool index_delete(index_t* index);
```

### word
Detailed descriptions of each function are provided above and are not repeated here.
```text
char* normalizeWord(const char* word);
```

### pagedir (common)
Used by the indexer to load webpages from crawler output.
```text
webpage_t* pagedir_load(const char* pageDirectory, const int docID);
```

## Error handling and recovery
All problems are reviewed accordingly and produce a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the mem_assert functions, which result in a message printed to stderr and a non-zero exit status. We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the mem_assert functions), e.g., if a function receives bad parameters.
