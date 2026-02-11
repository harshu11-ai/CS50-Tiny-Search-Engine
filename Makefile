# Makefile for CS50 Tiny Search Engine
#
# David Kotz - April 2016, 2017, 2021

L = libcs50
.PHONY: all clean test valgrind

############## default: make all libs and programs ##########
# If libcs50 contains set.c, we build a fresh libcs50.a;
# otherwise we use the pre-built library provided by instructor.
all: 
	(cd $L && if [ -r set.c ]; then make $L.a; else cp $L-given.a $L.a; fi)
	make -C common
	make -C crawler
	# make -C indexer
	# make -C querier

############## test: run crawler test script ##########
test: all
	./crawler/testing.sh

############## valgrind: run crawler under valgrind ##########
valgrind: all
	make -C crawler valgrind

############### TAGS for emacs users ##########
TAGS:  Makefile */Makefile */*.c */*.h */*.md */*.sh
	etags $^

############## clean  ##########
clean:
	rm -f *~
	rm -f TAGS
	make -C libcs50 clean
	make -C common clean
	make -C crawler clean
	# make -C indexer clean
	# make -C querier clean
