#!/bin/bash

# Compile with optimization level 2, this enables tail recursion. use -Os for max optimization
# g++ -fopenmp -O2 test.cc

#
# ./a.out 16   0.13 data/realWorldSets.txt
# ./a.out 32   0.13 data/realWorldSets.txt
# ./a.out 64   0.13 data/realWorldSets.txt
# ./a.out 128  0.13 data/realWorldSets.txt
# ./a.out 256  0.13 data/realWorldSets.txt
# ./a.out 512  0.13 data/realWorldSets.txt
# ./a.out 1024 0.13 data/realWorldSets.txt

./a.out 16   0.13 data/best8korkut.txt
./a.out 32   0.13 data/best8korkut.txt
./a.out 64   0.13 data/best8korkut.txt
./a.out 128  0.13 data/best8korkut.txt
./a.out 256  0.13 data/best8korkut.txt
./a.out 512  0.13 data/best8korkut.txt
./a.out 1024 0.13 data/best8korkut.txt