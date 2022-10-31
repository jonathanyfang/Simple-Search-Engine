# Jonathan Fang
## CS50 Fall 2022, Tiny Search Engine
### Functions
Parses through arguments:
```c
static void parseArgs(int argc, char *argv[]);
```

Wraps the entirety of the work of querier after parsing the arguments. 
```c
static void querier(char* pageDirectory, char* indexFilename, index_t *index);
```

Only prints when stdin is a user, not during testing
```c
int fileno(FILE *stream); // given in instructions
static void prompt(void); // given in instructions
```

Once the query has been tokenized and validated, score_query deals with all the operators and prints out the results as detailed in the requirement specs
```c
static void score_query(char* tokens[], index_t *index, int word_count, char* pageDirectory);
```

Used to take the union of two counters with the `counters_iterate` method. Will only change the first counter. Modified from the code given.
```c
static void counters_merge(counters_t* result, counters_t* another);
static void counters_merge_helper(void* arg, const int key, const int item);
```

Used to take the intersection of two counters with the `counters_iterate` method. Will only change the first counter.
Modified from the code given.
```c
static void counters_intersect(counters_t* ct1, counters_t* ct2); 
static void intersect_helper(void *arg, const int key, const int count);
```

Two more iterators helper functions. The first helps to count the number of documents matched by the query. get_ranking returns the docID and highest score in a counters.
```c
static void match_count(void* arg, const int key, int item);
static void get_ranking(void* arg, const int docID, int score);
```


### Included modules
Included `index`, `pagedir`, and `words`.
Added two new functions to `words`:
``` c
// Parses through and makes sure the string is only letters and spaces
// If so, tokenize the query string (char* line)
bool tokenizeString(char* line, char* tokenArr[], int* len);

// Validates the query:
// Neither the first or last word may be an operator, and two operators may not be adjacent. 
// If valid, return true; otherwise print a suitable error message and return false.
bool validateQuery(char* tokenArr[], int len);
```

### Pseudocode
Followed pseudocode given in the requirement specs
```
https://www.cs.dartmouth.edu/~tjp/cs50/labs/lab6/REQUIREMENTS
```

### Error handling
Any errors should print to `stderr` and exit. 

### Testing
Wrote a bash script testing.sh to test break cases for `querier`. I first tested various invalid queries, then tested proper queries against the toscrape at depth 1 we were given in the instructions. Finally, I used the fuzzquery to test a random set of queries on letters at depth 1.

 I also tested for memory leaks using `make valgrind`.

### Valgrind
Debugged and tested for memory leaks with valgrind. 

