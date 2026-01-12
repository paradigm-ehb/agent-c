#!bin/sh

set -eu

clang++ -std=c++20 configure.cpp pkgm_main.cpp -ldnf5 -o nob
./nob
