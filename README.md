[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/64UnCnx-)

# Tiny Search Engine (TSE)

**Author:** Harshith Yallampalli  
**Created for:** Dartmouth CS50

This project was created for my CS50 class as part of the Tiny Search Engine (TSE) assignment. The TSE is a multi-module system that implements a simplified web search engine, similar to how programs like Google crawl, index, and search the web.

## Project Overview

The Tiny Search Engine consists of several components that work together to build a searchable index of web pages. This repository currently implements the **crawler** module—the component responsible for systematically exploring the web starting from a seed URL and saving discovered pages to disk.

### Components

- **crawler** — Crawls the web from a given seed URL to a specified depth, saving each fetched webpage to a page directory. Uses a bag for the frontier of URLs to explore and a hashtable to track visited URLs and avoid duplicates.
- **common** — Shared utilities including `pagedir` for initializing page directories and saving webpages in a consistent format.
- **libcs50** — The CS50 library providing data structures (bag, hashtable, set) and utilities (webpage, file, memory).

## Building and Running

```bash
make          # Build all components
make test     # Run the crawler test script
make valgrind # Run crawler under valgrind
make clean    # Remove build artifacts
```

When performing tests, run:
```bash
make test &> crawler/testing.out
```

## Implementation Notes

- **Assumptions:** The crawler implementation follows the specification requirements; no additional assumptions were made.
- **Testing:** Testing for toscrape at depth 2 is not included in the test suite since it takes too long to accurately observe performance.
- **Known Issue:** There is a memory leak ("still reachable: 7,215 bytes in 16 blocks") present. No alterations to the written code have resolved this error so far.
- **Compiler Warning:** There is a warning upon compiling `crawler.c` relating to casting a pointer from an integer of different size (line 40). This is a warning only and does not affect the ability of the code to run. 
