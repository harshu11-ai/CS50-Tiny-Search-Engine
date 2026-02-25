# CS50 Tiny Search Engine (TSE)

This repository is for my **CS50 search engine assignment**.  
The project builds a small search engine in C with three main parts: a crawler, an indexer, and a querier.

## Assignment Specs (Summary)

1. **Crawler**
- Crawls pages starting from a seed URL.
- Follows only internal links.
- Respects the max crawl depth.
- Saves fetched pages in a page directory.

2. **Indexer**
- Reads saved pages from the crawler output.
- Parses and normalizes words.
- Builds an inverted index mapping words to document IDs and counts.
- Writes the index to a file.

3. **Querier**
- Loads the index and page directory.
- Accepts queries from standard input.
- Supports `and` / `or` query logic.
- Ranks and prints matching documents by score.

## Assumptions / Notes

- The crawler seed URL is assumed to be valid and internal to the CS50 site being crawled.
- `maxDepth` is assumed to be within the allowed range (`0` to `10`).
- The indexer ignores words shorter than 3 characters when building the index.
- Queries are assumed to be space-separated words using `and` / `or` operators, and are expected in lowercase.
- Query input is assumed to stay within the tokenizer limit (up to 100 tokens per query line).
- Testing for `toscrape` depth 2 is not included in regular tests because it takes too long to run.
- A known Valgrind report remains: `still reachable: 7,215 bytes in 16 blocks`.

## Build and Test

- Build all components:
```bash
make
```
- Run component tests:
```bash
make test
```
