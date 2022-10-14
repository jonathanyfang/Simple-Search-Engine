#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../common/pagedir.h"
#include "../libcs50/webpage.h"

/* 
* crawler.c
* 
* The crawler crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts
* any embedded URLs and retrieves those pages, and crawls the pages found at those URLs, but limiting itself to some threshold number 
* of hops from the seed URL (the ‘depth’), and avoiding visiting any given URL more than once. It saves the pages, and the URL and 
* depth for each, in files. When the crawler process is complete, the indexing of the collected documents can begin.
* 
* Jonathan Fang
* CS50, Fall 2022
*/

// Uncomment the line below to run the logr functions 
// #define APPTEST 1
const int HT_SIZE = 50;

static void crawler(char* seedURL, char* pageDirectory, int maxDepth);
static void pageScan(webpage_t *page, hashtable_t *ht, bag_t *bag);
static int parseArgs(int argc, char* argv[]);
static void logr(const char *word, const int depth, const char *url);

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
static int parseArgs(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n");
        exit(1);
    }
    // if the seed URL isn't internal, no need to crawl
    char *seedURL = argv[1];
    if (!isInternalURL(seedURL)) {
            fprintf(stderr, "This url isn't internal\n");
            exit(2);
        }
    // call pagedir_init()
    char* pageDirectory = argv[2];
    if (!pagedir_init(pageDirectory)) {
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
static void crawler(char* seedURL, char* pageDirectory, int maxDepth) {
    // make a webpage for the seedURL, marked with depth=0
    webpage_t* page;
    char* seedcopy = malloc(strlen(seedURL) + 1);
    strcpy(seedcopy, seedURL);
    page = webpage_new(seedcopy, 0, NULL);
    if (page == NULL) {
        fprintf(stderr, "error allocating webpage\n");
        free(seedcopy);
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
            logr("Fetched", webpage_getDepth(page), webpage_getURL(page));
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
static void pageScan(webpage_t *page, hashtable_t *pages_seen, bag_t *pages_to_crawl) {
    int depth = webpage_getDepth(page) + 1;
    logr("Scanning", webpage_getDepth(page), webpage_getURL(page));
    char* result;
    // initial position
    int pos = 0; 
    // change value of pos during the while loop
    while ((result = webpage_getNextURL(page, &pos)) != NULL) {
            // if that URL is not ‘internal’
        if(isInternalURL(result)) {
            logr("Found", depth, result);
            // if hashtable_insert is true, means that the page has not been inserted into pages_seen or pages_to_crawl
            if (hashtable_insert(pages_seen, result, "")) {
                logr("Added", depth, result);
                // copy string, so the url in hashtable doesn't point to the url in newPage
                webpage_t *newPage = webpage_new(result, webpage_getDepth(page) + 1, NULL);
                bag_insert(pages_to_crawl, newPage);
            }
            else{
                logr("IgnDupl", depth, result);
                free(result);
            }
        }
        // If we do not create a new webpage, we must clean up the string here, as it won't be deleted by webpage_delete
        else {
            logr("IgnExtrn", depth, result);
            fprintf(stderr, "url %s is not internal\n", result);
            free(result);
        }
    }
}

// log one word (1-9 chars) about a given url                                   
static void logr(const char *word, const int depth, const char *url)
{
#ifdef APPTEST
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
#else
  ;
#endif
}
