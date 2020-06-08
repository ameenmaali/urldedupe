#!/bin/bash
# Script for local development - simple build and run

clang++ *.cpp -std=c++17
./a.out -u $1 $2
