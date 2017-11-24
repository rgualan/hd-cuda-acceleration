#!/bin/bash

#if [ -e $1 ] ;then read str; else str=$1; fi
#len=`expr length $str`
#echo $len;
#for (( a=0; a<=$len; a=a+2 )); do echo -n ${str:a:2}" "; done
#echo ""

#echo $str;

#while read line ; do
#    echo $line;
#done

awk '{echo $1}'
