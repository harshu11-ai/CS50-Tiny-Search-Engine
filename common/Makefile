# Makefile for common directory
# Author: Harshith Yallampalli

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50

OBJS = pagedir.o index.o word.o
LIB = libcommon.a

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

pagedir.o: pagedir.c pagedir.h ../libcs50/file.h
	$(CC) $(CFLAGS) -c pagedir.c

index.o: index.c index.h ../libcs50/hashtable.h ../libcs50/counters.h
	$(CC) $(CFLAGS) -c index.c

word.o: word.c word.h
	$(CC) $(CFLAGS) -c word.c

.PHONY: all clean
clean:
	rm -f $(OBJS) $(LIB)