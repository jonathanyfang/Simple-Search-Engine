# Testing script for querier.c
#
# usage: bash -v testing.sh
#
# Jonathan Fang
# CS50, Fall 2022

if [ ! -z "$1" ]; then
  if [ $1 == "VG" ]; then
    VALGRIND="valgrind --leak-check=full --show-leak-kinds=all" 
    echo The following tests will check for memory leaks.
  fi 
else
  VALGRIND=""
  echo The following tests will run without checking for memory leaks.
fi

givenDir="/thayerfs/courses/22fall/cosc050/workspace/tse/tse-output"

# Test case 1
pageDirectory1="${givenDir}/toscrape-depth-1"
indexFilename1="${givenDir}/toscrape-index-1"

# Test case 2
pageDirectory2="${givenDir}/letters-depth-1"
indexFilename2="${givenDir}/letters-index-1"

######################################
### These tests should pass ####
### If nothing prints, no errors

### Run querier on toscrape depth 1. These will all be invalid queries
touch invalid_queries
echo "These will be invalid queries; none should match" >> invalid_queries
echo "  " >> invalid_queries
echo "" >> invalid_queries
echo "This should fail because I have 1 numerical digit" >> invalid_queries
echo "And this one has a starting operator" >> invalid_queries
echo "This one does to or" >> invalid_queries
echo "This one has and and or next to each other" >> invalid_queries
$VALGRIND ./querier $pageDirectory1 $indexFilename1 < invalid_queries
rm -f invalid_queries

### Run querier on toscrape depth 1. These should match the given outputs
touch queries
echo "Europe travel" >> queries
echo "Europe and travel" >> queries
echo "Europe or travel" >> queries
echo "exit" >> queries
$VALGRIND ./querier $pageDirectory2 $indexFilename2 < queries
rm -f queries

### Fuzzy testing
./fuzzquery /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/letters-index-1 20 1 | ./querier $pageDirectory2 $indexFilename2

### etc.