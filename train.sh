#!/bin/bash

if [ -f "./main" ]; then
    rm main
fi
c++ main.cpp -o main

python ./train.py

wait
