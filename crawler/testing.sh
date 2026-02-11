#!/bin/bash
# Testing script for crawler
# Author: Harshith Yallampalli
PROG="crawler/crawler"
DATA="data"
SEED_BASE="http://cs50tse.cs.dartmouth.edu/tse"

mkdir -p "$DATA"

echo "1) Bad Arguments Tests"
$PROG || true
$PROG "$SEED_BASE/letters/index.html" "$DATA/letters-arg" || true
$PROG "https://cs50tse.cs.dartmouth.edu/tse/letters/index.html" "$DATA/letters-arg" 1 || true
mkdir -p "$DATA/letters-arg"
$PROG "$SEED_BASE/letters/index.html" "$DATA/letters-arg" -5 || true
$PROG "$SEED_BASE/letters/index.html" "$DATA/letters-arg" 15 || true

echo "2) Letters Depths 0,1,2"
mkdir -p "$DATA/letters-0" "$DATA/letters-1" "$DATA/letters-2"
$PROG "$SEED_BASE/letters/index.html" "$DATA/letters-0" 0
$PROG "$SEED_BASE/letters/index.html" "$DATA/letters-1" 1
$PROG "$SEED_BASE/letters/index.html" "$DATA/letters-2" 2

echo "3) Letters with Different Seeds"
mkdir -p "$DATA/letters-A" "$DATA/letters-E"
$PROG "$SEED_BASE/letters/A.html" "$DATA/letters-A" 4
$PROG "$SEED_BASE/letters/E.html" "$DATA/letters-E" 4

echo "4) Toscrape Depths 0, 1"
mkdir -p "$DATA/toscrape-0" "$DATA/toscrape-1"
$PROG "$SEED_BASE/toscrape/index.html" "$DATA/toscrape-0" 0
$PROG "$SEED_BASE/toscrape/index.html" "$DATA/toscrape-1" 1

echo "5) Letters Depth 3"
mkdir -p "$DATA/letters-3"
$PROG "$SEED_BASE/letters/index.html" "$DATA/letters-3" 3

echo "done"