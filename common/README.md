# Common directory
This directory will hold code used by multiple modules in the Tiny Search Engine (TSE).

## Usage
To start run the command:
```bash
$ make
```
This will build the `common.a` library.

## Functionalities
`pagedir.c` is a re-usable module that implements some of our directory functions

### `pagedir`
Now, we describe the functions within the `pagedir` module. 

##### `pagedir_init`
checks if the directory is writable/exists, returns a boolean value
```C
bool pagedir_init(char *pageDirectory);
```

##### `pagedir_save`
saves the webpage information into a file with a unique ID in the pageDirectory
```C
void pagedir_save(webpage_t *page, int id, char *pageDirectory;
```