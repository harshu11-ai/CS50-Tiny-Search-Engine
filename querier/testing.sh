#!/bin/bash
# Testing script for querier.
# Run from project root: ./querier/testing.sh

PROG="./querier/querier"
DATA="data"
PAGEDIR="$DATA/toscrape-2"
INDEXFILE="$DATA/index-toscrape-2.out"

mkdir -p "$DATA"

echo "1) querier - invalid arguments"
$PROG 2>&1 || true
$PROG "$PAGEDIR" 2>&1 || true
$PROG "$PAGEDIR" "$INDEXFILE" "extra" 2>&1 || true

echo "1.1) querier - invalid pageDirectory"
$PROG "$DATA/nonexistent" "$INDEXFILE" 2>&1 || true
$PROG "." "$INDEXFILE" 2>&1 || true

echo "1.2) querier - invalid indexFile"
$PROG "$PAGEDIR" "$DATA/nonexistent-index.out" 2>&1 || true
touch "$DATA/readonly-q.out" 2>/dev/null || true
chmod 000 "$DATA/readonly-q.out" 2>/dev/null || true
$PROG "$PAGEDIR" "$DATA/readonly-q.out" 2>&1 || true
chmod 644 "$DATA/readonly-q.out" 2>/dev/null || true

echo "2) querier - uninteresting / invalid queries"
$PROG "$PAGEDIR" "$INDEXFILE" <<EOF


test!
and test
test or
and or
test and and test
EOF

echo "3) querier - simple valid queries"
$PROG "$PAGEDIR" "$INDEXFILE" <<EOF
religion
history
religion and history
religion or history
EOF

echo "querier testing complete."

