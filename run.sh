#!/bin/bash

c++ main.cpp -o main

if ! read -t 2 -p "Path of map: " map_path; then
    paths=$(ls "./maps" | sort)
    paths_array=($paths)
    map_path="./maps/${paths_array[-1]}"
fi

./PreliminaryJudge ./main -m $map_path

./replayer/CodeCraft_2024_Replay.x86_64

wait
