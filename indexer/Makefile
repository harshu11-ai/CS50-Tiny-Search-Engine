# Makefile for indexer and indextest
# Author: Harshith Yallampalli

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
LFLAGS = -L../common -lcommon -L../libcs50 -lcs50-given -lm

all: indexer indextest

indexer: indexer.o
	$(CC) $(CFLAGS) indexer.o -o indexer $(LFLAGS)

indextest: indextest.o
	$(CC) $(CFLAGS) indextest.o -o indextest $(LFLAGS)

indexer.o: indexer.c
	$(CC) $(CFLAGS) -c indexer.c

indextest.o: indextest.c
	$(CC) $(CFLAGS) -c indextest.c

.PHONY: clean valgrind

clean:
	rm -f indexer indextest indexer.o indextest.o

valgrind: indexer indextest
	mkdir -p ../data
	valgrind --leak-check=full --show-leak-kinds=all ./indexer ../data/letters-3 ../data/valgrind-index.out
	valgrind --leak-check=full --show-leak-kinds=all ./indextest ../data/valgrind-index.out ../data/valgrind-index2.out
