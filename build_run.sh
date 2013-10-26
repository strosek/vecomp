#!/bin/bash

clear

if [ -e errores.out ]; then
  rm errores.out
fi

flags='-Wall -Wextra -Wc++11-compat -Wswitch -Wshadow -Weffc++ -g -std=c++11'

g++ $flags -o binaries/vecomp code/source/main.cpp code/source/parser.cpp \
    code/source/scanner.cpp \
    code/source/tokenlexeme.cpp

if [ $? -eq 0 ]; then
  binaries/vecomp $1 
fi

if [ $? -eq 0 ]; then
  echo -e "\n[Program exited with SUCCESS status]"
else
  echo -e "\n[Program exited with FAILURE status]"
fi

