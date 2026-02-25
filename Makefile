# Makefile for CS50 Tiny Search Engine
#
# David Kotz - April 2016, 2017, 2021

L = libcs50
.PHONY: all clean test-crawler test-indexer test-querier valgrind-crawler valgrind-indexer valgrind-querier

############## default: make all libs and programs ##########
# If libcs50 contains set.c, we build a fresh libcs50.a;
# otherwise we use the pre-built library provided by instructor.
all: 
	(cd $L && if [ -r set.c ]; then make $L.a; else cp $L-given.a $L.a; fi)
	make -C common
	make -C crawler
	make -C indexer
	make -C querier

############## test: run crawler test script ##########
test-crawler: all
	./crawler/testing.sh

############## test-indexer: run indexer and indextest tests ##########
test-indexer: all
	./indexer/testing.sh

############## test-querier: run querier test script ##########
test-querier: all
	./querier/testing.sh

############## valgrind: run crawler under valgrind ##########
valgrind-crawler: all
	make -C crawler valgrind

############## valgrind-indexer: run indexer and indextest under valgrind ##########
valgrind-indexer: all
	make -C indexer valgrind

############## valgrind-querier: run querier under valgrind ##########
valgrind-querier: all
	make -C querier valgrind

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
	make -C indexer clean
	make -C querier clean
