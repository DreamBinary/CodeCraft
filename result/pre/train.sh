#!/bin/bash

module load anaconda/2022.10
module load gcc/11.2

source activate codecraft

if [ -f "./main" ]; then
    rm main
fi
c++ main.cpp -o main

python ./train.py

wait
