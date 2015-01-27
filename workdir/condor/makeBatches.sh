#!/bin/bash

processes="W3J W4J"
splitLevel=200

for p in $processes; do
   i=0
   while [ $i -lt $splitLevel ] ; do
      s=$i"of"$splitLevel
      i=`expr $i + 1`
      arguments="-s$s -i$p -o/data/users/kkotov/2015.01.27/"
      cat batch.jdf | sed -e "s|ARGS|$arguments|g" -e "s|OUT|$p$s|g" -e "s|ERR|$p$s|g" -e "s|LOG|$p$s|g" > tmp/batch$p""$s.jdf
   done;
done;
