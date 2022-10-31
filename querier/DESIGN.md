# Jonathan Fang
## CS50 Fall 2022, Tiny Search Engine
### Design

### Data Structures:
We use a couple of local data structures to help store information while we run the iterators. `twocts` stores two counters during the unions and intersection functions, and `id_and_score` helps to store the highest scoring docID and score as we sort the query results.

### Pseudo code for logic/algorithmic flow
The querier will run as follows:

1. Execute from a command-line with usage syntax `./querier pageDirectory indexFilename`
2. Parse the command-line arguments:
    1. Ensure there were exactly two arguments
    2. Ensure `pageDirectory` is a crawler produced directory using `pagedir_validate`
    3. `indexFilename` is the pathname of a file that can be read
3. Begin reading from stdin, until you reach EOF
	1. Tokenize/validate the line using `tokenizeString` and `validateQuery`
    2. Score the query using the given pseudocode for dealing with operators
	3. Iterate through the resulting counters to check the number of matched documents
	4. Iterate once again and sort to get a ranking of matched documents
    
    5. print the score, docID, and URL
6. Exit zero when EOF is reached on stdin