# Testing script for crawler.c
#
# usage: bash -v testing.sh
#
# Jonathan Fang
# CS50, Fall 2022

testDir="./index_output"
unreadableDir="./index_output/unreadable"
unwritableDir="./index_output/unwritable"
unwritableIndexFile="./index_output/unreadable_file"

if ! [[ -d "$testDir" ]]; then
  mkdir "$testDir"
fi

if ! [[ -d "$unreadableDir" ]]; then
  mkdir "$unreadableDir"
  chmod -r $unreadableDir
fi

if ! [[ -d "$unwritableDir" ]]; then
  mkdir "$unwritableDir"
  chmod -w $unwritableDir
  chmod +r $unwritableDir
fi

if ! [[ -f "$unwritableIndexFile" ]]; then
  touch "$unwritableIndexFile"
  chmod -w $unwritableIndexFile
  chmod +r $unwritableIndexFile
fi



# Define variables
pageDirectory1="../crawler/test_output/wikipedia/depth1"
givenFilename1="/thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/wikipedia-index-1"
indexFilename1="index_output/wikipedia-indexer-1"
newIndexFilename1="index_output/wikipedia-indextest-1"
pageDirectory2="../crawler/test_output/letters/depth4"
givenFilename2="/thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/letters-index-4"
indexFilename2="index_output/letters-indexer-4"
newIndexFilename2="index_output/letters-indextest-4"


#####################################
### These tests should fail ###

# 0 argument
./indexer

# 1 arguments
./indexer $pageDirectory1

# 3 arguments
./indexer $pageDirectory1 $indexFilename1 $pageDirectory1

# invalid pageDirectory (non-existent path)
./indexer ../nonexistent ${indexFilename1}

# invalid pageDirectory (not a crawler directory)
./indexer ${testDir} ${indexFilename1}

# invalid pageDirectory (not readable)
./indexer ${unreadableDir} ${indexFilename1}

# invalid indexFile (non-existent path)
/indexer $pageDirectory1 nonexistentPath/test

# invalid indexFile (read-only directory)
./indexer ${pageDirectory1} ${unwritableDir}

# invalid indexFile (existing, read-only file)
./indexer $pageDirectory1 $unwritableIndexFile

######################################
### These tests should pass ####
### If nothing prints, no errors

### Run indexer on wikipedia depth 1
./indexer $pageDirectory1 $indexFilename1
./indextest $indexFilename1 $newIndexFilename1
### Compare the two
/thayerfs/courses/22fall/cosc050/workspace/tse/indexcmp $indexFilename1 $newIndexFilename1

### Run indexer on letters depth 4
./indexer $pageDirectory2 $indexFilename2
./indextest $indexFilename2 $newIndexFilename2
### Compare the two
/thayerfs/courses/22fall/cosc050/workspace/tse/indexcmp $indexFilename2 $newIndexFilename2


### etc.