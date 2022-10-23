# Jonathan Fang
## CS50 Fall 2022, Tiny Search Engine
### Implementation
The indexer follows basic logic.
1. Execute from the command line
2. Process and validate command-line parameters
3. Call `index_build` and get back an index data structure
    1. Create a new index using `index_new`
    2. Loop through DocIds from 1, and load pages from the directories using `pagedir_load` from the `pagedir.h` module
    2. Loop through the html in the page using webpage_getNextWord from the `webpage.h` module
    3. Insert the words into the index
5. Save the index to `indexFilename`
6. Delete the index to clean up memory

### Inputs
The indexer requires exactly two command-line parameters:
- `pageDirectory` is a path to a crawler directory. We use `pagedir_validate` to make sure this is the case.
y that is created by the crawler. We assume that this directory is correctly produced, such that the files are named 1, 2, 3, …, without gaps.
- `indexFilename` is the path to the file that we will save the indexed output into.

The indextest program requires two command-line arguments:
- `oldIndexFilename` is the path to an index-outputted file. We assume that the file is truly an index file is properly formatted.
- `newIndexFilename` is the path to the newly initlized index file.

### Outputs

There should be no output to stdout. The file that the index is saved to should follow the format for each line:
```
word docID count [docID count]...
```

### Included modules
I created an index data structure that wraps a hashtable of counters in `index.h`. I also include the `word.h` and `pagedir.h` modules in the `common` directory.

### Error handling
Any errors should print to `stderr` and exit. 

### testing.sh
Wrote a bash script testing.sh to test break cases for `indexer`. I copied the index results given to us for wikipedia at a depth of 1. I then ran `indexer` on my `crawler` output for wikipedia at a depth of 1. I then ran `indextest` on the `indexer` output. Next, I used the given `indexcmp` function. 

I ran this again, but with the letters results at a depth of 4.
Also tested the `indexcmp` manually.

### valgrind
Debugged and tested for memory leaks with valgrind. 

