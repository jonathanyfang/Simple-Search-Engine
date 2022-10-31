# Jonathan Fang
## CS50 Fall 2022, Tiny Search Engine

To read more about implementation and testing, look at `IMPLEMENTATION.md`.

### Usage
```bash
./querier pageDirectory indexFilename
```

### Assumptions
The crawler directory has no more than 4 digit docIDs (at most 9999 crawler produced files).
There are at most 100 words in each query.

### Compilation
Compile using the following command:
```bash
make 
```
This will make querier and the fuzzquery executables.
Test and create a testing.out file using:
```bash
make test 
```
Test for memory leaks and create a testing.out file using:
```bash
make valgrind
```
Clean up using:
```bash
make clean
```