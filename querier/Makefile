# Makefile for the querier directory
#
# Jonathan Fang
# CS50, Fall 2022

# Makefile for the indexer directory
#
# Jonathan Fang
# CS50, Fall 2022

OBJS1 = querier.o
OBJS2 = fuzzquery.o
LIBS = ../libcs50/libcs50.a ../common/common.a


# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

MAKE = make
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: test-valgrind clean all

all: querier fuzzquery

querier: $(OBJS1) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

querier.o: 

fuzzquery: $(OBJS2) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

fuzzquery.o: 

test: querier
	bash -v testing.sh &> testing.out

# Usage: make valgrind
valgrind:
	bash -v testing.sh VG &> testing.out

clean:
	rm -f test/*
	rm -f *.o
	rm -f querier
	rm -f fuzzquery
	rm -f core