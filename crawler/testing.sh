# Testing script for crawler.c
# Author: Jonathan Fang
# Date: Oct 2022
#
# usage: bash -v testing.sh

# Define variables
seedURL="http://cs50tse.cs.dartmouth.edu/tse/letters/"
seedURL2="http://cs50tse.cs.dartmouth.edu/tse/wikipedia/"
externalURL="www.google.com"
testDir="./test_output"
t="test"

if ! [[ -d "$t" ]]; then
  mkdir "$t"
fi

if ! [[ -d "$testDir" ]]; then
  mkdir "$testDir"
fi

lettersDir="./test_output/letters"
wikipediaDir="./test_output/wikipedia"
toscrapeDir="./test_output/toscrape"

if ! [[ -d "$lettersDir" ]]; then
  mkdir "$lettersDir"
fi

if ! [[ -d "$wikipediaDir" ]]; then
  mkdir "$wikipediaDir"
fi

if ! [[ -d "$toscrapeDir" ]]; then
  mkdir "$toscrapeDir"
fi


#####################################
### These tests should fail ###

# 1 argument
./crawler

# 2 arguments
./crawler $seedURL

# 3 arguments
./crawler $seedURL $t

# 4 arguments + externalURL
./crawler $externalURL $t 2

# 4 arguments, negative depth
./crawler $seedURL $t -1

# 4 arguments, greater than max depth
./crawler $seedURL $t 11

# Directory does not exist
./crawler $seedURL dne 2

######################################
### These tests should pass ####

for depth in {0..5}; do
  if ! [[ -d "${lettersDir}/depth${depth}" ]]; then 
      mkdir "${lettersDir}/depth${depth}"   
  fi
  ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ${lettersDir}/depth${depth} $depth
done

for depth in {0..1}; do
  if ! [[ -d "${wikipediaDir}/depth${depth}" ]]; then 
      mkdir "${wikipediaDir}/depth${depth}"   
  fi
  ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ${wikipediaDir}/depth${depth} $depth
done

# Tests repeated links
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ${toscrapeDir} 1
# Commented out since it takes a while to run, but tested it when I created testing.out