#!/bin/bash

for i in {0..3}; do 
  echo $i;
  (cd $i; ./tabulate.sh);
done;
