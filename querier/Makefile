# Makefile for querier
# Author: Harshith Yallampalli

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
LFLAGS = -L../common -lcommon -L../libcs50 -lcs50-given -lm

PROG = querier
OBJS = querier.o
TESTSCRIPT = testing.sh

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG) $(LFLAGS)

querier.o: querier.c
	$(CC) $(CFLAGS) -c querier.c

.PHONY: clean test valgrind

clean:
	rm -f $(PROG) $(OBJS)

test: $(PROG) $(TESTSCRIPT)
	./$(TESTSCRIPT)

valgrind: $(PROG)
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROG) ../data/letters-3 ../data/index-letters-3.out

