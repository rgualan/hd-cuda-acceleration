#!/bin/bash

file='tmp98796179.txt';

#Put the pipe input data in a temporal file
( while read x; do echo $x; done ) > $file;

#Show input data
echo "Analysing pipe input...";
cat $file;

#Calculate the average using awk
echo "Averaging...";
awk '{ total += $1; count++ } END { print total; print count; print total/count; }' $file;


