# Makefile for crawler
# Author: Harshith Yallampalli

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
LFLAGS = -L../libcs50 -lcs50-given -L../common -lcommon -lm
PROG = crawler
OBJS = crawler.o

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG) $(LFLAGS)

crawler.o: crawler.c crawler.h
	$(CC) $(CFLAGS) -c crawler.c

.PHONY: clean test valgrind

clean:
	rm -f $(PROG) $(OBJS)

test: $(PROG)
	./testing.sh

valgrind: $(PROG)
	mkdir -p ../data/valgrind-letters-1 && valgrind --leak-check=full --show-leak-kinds=all ./$(PROG) http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/valgrind-letters-1 1