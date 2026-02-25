#!/bin/bash
# Testing script for indexer and indextest.
# Run from project root: ./indexer/testing.sh

INDEXER="./indexer/indexer"
INDEXTEST="./indexer/indextest"
DATA="data"
PAGEDIR="$DATA/letters-3"
INDEXFILE="$DATA/index.out"
NEWINDEX="$DATA/index2.out"

echo "1) Indexer - invalid arguments"
$INDEXER 2>&1 || true
$INDEXER "$PAGEDIR" 2>&1 || true
$INDEXER "$PAGEDIR" "$INDEXFILE" extra 2>&1 || true

echo "2) Indexer - invalid pageDirectory (non-existent)"
$INDEXER "$DATA/nonexistent" "$INDEXFILE" 2>&1 || true

echo "3) Indexer - invalid indexFile"
touch "$DATA/readonly.out" 2>/dev/null || true
chmod 444 "$DATA/readonly.out" 2>/dev/null || true
$INDEXER "$PAGEDIR" "$DATA/readonly.out" 2>&1 || true
chmod 644 "$DATA/readonly.out" 2>/dev/null || true

echo "4) Integration: indexer + indextest + diff"
$INDEXER "$DATA/letters-2" "$DATA/index-letters-2.out"
$INDEXER "$DATA/letters-3" "$DATA/index-letters-3.out"
$INDEXER "$DATA/toscrape-2" "$DATA/index-toscrape-2.out"
$INDEXTEST "$DATA/index-letters-2.out" "$DATA/index-letters-2-2.out"
$INDEXTEST "$DATA/index-letters-3.out" "$DATA/index-letters-3-2.out"
$INDEXTEST "$DATA/index-toscrape-2.out" "$DATA/index-toscrape-2-2.out"

echo "All tests passed" 

