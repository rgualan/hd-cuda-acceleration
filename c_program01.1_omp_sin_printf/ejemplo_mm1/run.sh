#!/bin/bash

for program in $(ls *.exe); do 
printf "\n\nRunning $program ...\n\n"; 
time ./$program; 
done;
