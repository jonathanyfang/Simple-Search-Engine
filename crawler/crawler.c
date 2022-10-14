#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../common/pagedir.h"
#include "../libcs50/webpage.h"

const int HT_SIZE = 100;

void crawler(char* seedURL, char* pageDirectory, int maxDepth);
void pageScan(webpage_t *page, hashtable_t *ht, bag_t *bag);
int parseArgs(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    // convert maxDepth from char* to int
    int maxDepth;
    maxDepth = parseArgs(argc, argv); // Will exit early if any of the parameters have issues, returns argv[3] as an int
    crawler(argv[1], argv[2], maxDepth);
    printf("Crawler crawled sucessfully\n");
    return 0;
}

/* Given arguments from the command line, extract them into the function parameters; return only if successful.
* return 0: successful
* exit 1: more than 4 arguments
* exit 2: seedURL can't be normalized or isn't a internal url
* exit 3: directory exists, initialize it
* exit 4: makes sure argv[3] maxDepth is an int and is nonnegative
* exit 5: validate new webpage
* exit 6: validate new hashtable
* exit 7: validate new bag
*/ 
int parseArgs(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n");
        exit(1);
    }
    // if the seed URL isn't internal, no need to crawl
    char *seedURL = argv[1];
    if (normalizeURL(seedURL) == NULL) {
        fprintf(stderr, "This url can't be normalized\n");
        exit(2);
    }
    else{
        if (!isInternalURL(seedURL)) {
            fprintf(stderr, "This url isn't internal\n");
            exit(2);
        }
    }
    // call pagedir_init()
    char* pageDirectory = argv[2];
    if (!pagedir_init(pageDirectory)) {
        fprintf(stderr, "This directory isn't writable or doesn't exist\n");
        exit(3);
    }
    char test;
    int maxDepth;
    // for maxDepth, ensure it is an integer in specified range [0 ... 10]
    if ((!sscanf(argv[3], "%d%c", &maxDepth, &test)) == 1 || maxDepth < 0 || maxDepth > 10) {
        fprintf(stderr, "maxDepth must be an integer in the range [0... 10] \n");
        exit(4);
    }
    // return the int form of argv[3]
    return maxDepth;
}


/* uses a bag to track pages to explore, and hashtable to track pages seen; when it explores a page */
void crawler(char* seedURL, char* pageDirectory, int maxDepth) {
    // make a webpage for the seedURL, marked with depth=0
    webpage_t* page;
    char* seedcopy = malloc(strlen(seedURL) + 1);
    strcpy(seedcopy, seedURL);
    page = webpage_new(seedcopy, 0, NULL);
    if (page == NULL) {
        fprintf(stderr, "error allocating webpage\n");
        exit(5);
    }
    // initialize the hashtable and add the seedURL
    hashtable_t* pages_seen = hashtable_new(HT_SIZE);
    if (pages_seen == NULL) {
        fprintf(stderr, "error allocating hashtable\n");
        exit(6);
    }
    hashtable_insert(pages_seen, seedURL, "");
    
    // initialize the bag and add a webpage representing the seedURL at depth 0
    bag_t* pages_to_crawl = bag_new();
    if (pages_to_crawl == NULL) {
        fprintf(stderr, "error allocating bag\n");
        exit(7);
    }
    bag_insert(pages_to_crawl, page);
    // while bag is not empty, pull a webpage from the bag
    int id = 1;
    while ((page = bag_extract(pages_to_crawl)) != NULL) {
        // fetch the HTML for that webpage
        if (webpage_fetch(page)) {
            // if fetch was successful, save the webpage to pageDirectory
            pagedir_save(page, id, pageDirectory);
            id++;
            // if the webpage is not at maxDepth,
            if (webpage_getDepth(page) < maxDepth) {
                // pageScan that HTML
                pageScan(page, pages_seen, pages_to_crawl);
            }
        }
        // delete that webpage
        webpage_delete(page);
    }
    // delete the hashtable
    hashtable_delete(pages_seen, NULL);
    // delete the bag
    bag_delete(pages_to_crawl, NULL);
}

/* Given a webpage, scan the given page to extract any links (URLs), ignoring non-internal URLs; 
for any URL not already seen before (i.e., not in the hashtable), add the URL to both the hashtable pages_seen 
and to the bag pages_to_crawl */
void pageScan(webpage_t *page, hashtable_t *pages_seen, bag_t *pages_to_crawl) {
    char* result;
    // initial position
    int pos = 0; 
    // change value of pos during the while loop
    while ((result = webpage_getNextURL(page, &pos)) != NULL) {
            // if that URL is not ‘internal’
        if(isInternalURL(result)) {
            // if hashtable_insert is true, means that the page has not been inserted into pages_seen or pages_to_crawl
            if (hashtable_insert(pages_seen, result, "")) {
                // copy string, so the url in hashtable doesn't point to the url in newPage
                webpage_t *newPage = webpage_new(result, webpage_getDepth(page) + 1, NULL);
                bag_insert(pages_to_crawl, newPage);
            }
        }
        // If we do not create a new webpage, we must clean up the string here, as it won't be deleted by webpage_delete
        else {
            free(result);
            fprintf(stderr, "url %s is not internal\n", result);
        }
    }
}
