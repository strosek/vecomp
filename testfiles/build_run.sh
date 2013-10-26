#!/bin/bash

# Script for building vecomp project and execute a given test

clear

flags='-Wall -Wextra -Wc++11-compat -Wswitch -Wshadow -Weffc++ -g -std=c++11'

binary_dir='../binaries'
source_dir='../code/source'

source_files="$source_dir/main.cpp
              $source_dir/parser.cpp
              $source_dir/scanner.cpp
              $source_dir/tokenlexeme.cpp"


if [ ! -d $binary_dir ]; then
  mkdir $binary_dir
fi

g++ $flags -o $binary_dir/vecomp $source_files

if [ $? -eq 0 ]; then
  if [ -e errores.out ]; then
    rm errores.out
  fi

  $binary_dir/vecomp $1 
fi

if [ $? -eq 0 ]; then
  echo -e "\n[Program exited with SUCCESS status]"
else
  echo -e "\n[Program exited with FAILURE status]"
fi

