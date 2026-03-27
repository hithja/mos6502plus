#!/bin/bash
NAME="mosplus"

g++ ./src/*.cpp -I./include -o "$NAME"

if [[ "$1" == "run" ]]; then
    ./"$NAME"
fi