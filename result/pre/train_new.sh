#!/bin/bash

module load anaconda/3-Python3.9.10

source activate codecraft

if [ -f "./main" ]; then
    rm main
fi
c++ main.cpp -o main

python ./train.py

wait
