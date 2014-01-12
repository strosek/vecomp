#!/bin/bash

# Script for building vecomp project and execute a given test

clear

cd ../code
make

if [ $? -eq 0 ]; then
  if [ -f errores.out ]; then
    rm errores.out
  fi

  cd ../binaries
  ../binaries/vecomp $1 2> errores.out
fi

if [ $? -eq 0 ]; then
  echo -e "\n[Program exited with SUCCESS status]"
else
  echo -e "\n[Program exited with FAILURE status]"
fi

