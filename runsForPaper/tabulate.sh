#!/bin/bash

#Run 10 serial simulations and save a log
( for i in {1..10}; do echo $i; ./hd_driver.exe;  done; ) &> x10.log;
cat x10.log | grep "Call to kernel" | grep msec > runs.log;
rm -fr x10.log;

#Calculate average of runtime including IO operations
i=0; sum=0; 
for t in $(cat runs.log | grep -v "not" | cut -d: -f2 | cut -d" " -f2); do 
  sum=$( echo $sum+$t | bc); 
  i=$(($i+1)); 
done; 
avgIo=$(echo "scale=2; $sum/$i;" | bc); 


#Calculate average of runtime NOT including IO operations
i=0; sum=0; 
for t in $(cat runs.log | grep "not" | cut -d: -f2 | cut -d" " -f2); do 
  sum=$( echo $sum+$t | bc); 
  i=$(($i+1)); 
done; 
avgNoIo=$(echo "scale=2; $sum/$i;" | bc); 

echo "Promedio sin IO: $avgNoIo";
echo "Promedio con IO: $avgIo";

