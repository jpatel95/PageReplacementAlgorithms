#!/bin/bash

## Testing script
## For use with programs that use callng format as follows:
## ./replacement_alg size
## e.g. ./lru 10
##
## Call script using following format:
## /path/to/dir/test_one lru

## Place numbers.txt in same directory as the script 
## Also make sure that lru_faults.txt & lfu_faults.txt are
## also in the script file directory.
##
## While you may test your own page request files, I recommend using my
## provided test files and result files to verify your program
bigFILE=numbers.txt				#page request file

## do not create the following files; they will be created in the script
countFILE=counts.txt			#dummy file to log program output

## Change scriptDIR to path of directory with test files
## **** IF script is located w/in program directory, use ./ ****
scriptDIR=./

## generate count of fault results
for size in 10 50 100 250 500; do
	cat $scriptDIR$bigFILE | ./"$1" $size | grep -o -E [0-9]+ >> "$scriptDIR$countFILE"
done

## validate results against expected counts
DIFF=$(diff "$scriptDIR$1_$countFILE" $scriptDIR$countFILE)
if [ "$DIFF" == "" ]
then
	echo Success!
else
	echo "Something went wrong"
	echo "$DIFF"
	## clean up then exit script
	rm "$scriptDIR$countFILE"
	exit 1
fi

echo "Testing Done"

## clean up generated files and exit
rm $scriptDIR$countFILE

exit 0
