#!/bin/bash

# Script for building vecomp project without debugging info and optimized

flags='-Wall -Wextra -Wc++11-compat -Wswitch -Wshadow -Weffc++ -std=c++11 -O2'

binary_dir='../binaries'
source_dir='../code/source'

source_files="$source_dir/main.cpp
              $source_dir/Parser.cpp
              $source_dir/Scanner.cpp
              $source_dir/FileReader.cpp
              $source_dir/ErrorReporter.cpp
              $source_dir/TokenLexeme.cpp"


if [ ! -d $binary_dir ]; then
  mkdir $binary_dir
fi

g++ $flags -o $binary_dir/vecomp $source_files

