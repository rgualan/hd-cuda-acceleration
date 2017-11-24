#!/bin/bash

FILENAME='tmp.txt';


if [ -f $FILENAME ] ; then
	file=$FILENAME;
else
	if [ $# -eq 1 ] ; then
		file=$1;		
	else
		echo "Invalid input parameters!";
		exit;
	fi;
fi;

echo "Analysing file $file...";
cat $file;

echo "Averaging...";
awk '{ total += $1; count++ } END { print total; print count; print total/count; }' $file;


