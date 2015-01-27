#!/bin/bash

echo "Initializing the environment..."
cd /home/kkotov/CMSSW_5_3_24/src/framework/workdir/
eval `scram runtime -sh`
echo "Launching the job..."
python test.py $1 $2 $3 $4 $5
