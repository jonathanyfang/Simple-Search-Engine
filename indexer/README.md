# Jonathan Fang
## CS50 Fall 2022, Tiny Search Engine

To read more about implementation and testing, look at `IMPLEMENTATION.md`.

### Usage
```bash
./indexer pageDirectory indexFilename
```

### Assumptions
None beyond the set requirements.

### Compilation
Compile using the following command:
```bash
make 
```
This will make indexer and the indextest executables.
Test using:
```bash
make test 
```
Clean up using:
```bash
make clean
```
CHeck for memory leaks using:
```bash
make valgrind pageDirectory indexFilename
```