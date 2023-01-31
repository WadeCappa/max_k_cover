#!/bin/bash

# Compile with optimization level 2, this enables tail recursion. use -Os for max optimization
g++ -fopenmp -O2 test.cc

#
./a.out 50 0.13 sets.txt