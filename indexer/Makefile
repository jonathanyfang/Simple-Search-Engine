# Makefile for the indexer directory
#
# Jonathan Fang
# CS50, Fall 2022

OBJS1 = indexer.o
OBJS2 = indextest.o
LIBS = ../libcs50/libcs50.a ../common/common.a


# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

MAKE = make
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: test valgrind clean all

all: indexer indextest

indexer: $(OBJS1) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer.o: 

indextest: $(OBJS2) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest.o: 

test: indexer
	bash -v testing.sh

# Usage: make valgrind [pageDirectory] [indexFilename]
valgrind: indexer
	$(VALGRIND) ./indexer 

clean:
	rm -f test/*
	rm -f *.o
	rm -f indexer
	rm -f output/
	rm -f core