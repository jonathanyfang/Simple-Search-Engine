OBJS = crawler.o
LIBS = ../libcs50/libcs50.a ../common/common.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

MAKE = make
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

crawler: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: 

.PHONY: test valgrind clean

test: crawler
	bash -v testing.sh

valgrind: crawler testing.sh
	$(VALGRIND) ./crawler < testing.sh

clean:
	rm -f test/*
	rm -f *.o
	rm -f crawler
	rm -f core