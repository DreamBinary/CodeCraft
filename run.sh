#!/bin/bash

c++ main.cpp -o main

if [ -n "$1" ]; then
    map_path=$1
    map_name=$(basename $map_path)
else
    paths=$(ls "./maps" | sort)
    paths_array=($paths)
    map_name=${paths_array[-1]}
    map_path="./maps/${paths_array[-1]}"
fi

log_path="./logs/${map_name}_$(date +%Y%m%d%H%M%S).log"


./PreliminaryJudge ./main -m $map_path >> ${log_path}

./replayer/CodeCraft_2024_Replay.x86_64

wait

