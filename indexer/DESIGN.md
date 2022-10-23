# Jonathan Fang
## CS50 Fall 2022, Tiny Search Engine
### Design

### Data Structure: Inverted Index
Our index datatype wraps a hashtable. Each key in the hashtable has a corresponding counterset as an item. The counterset keeps the unique document ID as a key and the count of how many times it appeared in the document as an item. This helps our index keep track of the necessary information: words, how many times they appear, and in which documents they appear.

### Indexer Pseudocode
The indexer will first process and validate command-line parameters using the helper function `parseArgs`. If successful, it will call the indexer function.

The indexer function first initializes the index data structure then builds the index by looping through the files in the directory in `index_build`. Next, indexer saves the index to a file with the given format with `index_save`. 

Finally, the indexer deletes the index with `index_delete` and cleans up the memory.