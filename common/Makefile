# Makefile for common directory
# Author: Harshith Yallampalli

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50

OBJS = pagedir.o
LIB = libcommon.a

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

pagedir.o: pagedir.c pagedir.h
	$(CC) $(CFLAGS) -c pagedir.c

.PHONY: all clean
clean:
	rm -f $(OBJS) $(LIB)