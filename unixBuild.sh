#!/bin/bash
mkdir -p build
cd build
cmake -S ../ -B .
export MAKEFLAGS=-j$(nproc)
make && make Shaders && ./LveEngine
cd ..
