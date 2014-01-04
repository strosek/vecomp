#!/bin/bash

# Script for building vecomp project and execute a given test

if [ -e errores.out ]; then
  rm errores.out
fi

../binaries/vecomp $1 

if [ $? -eq 0 ]; then
  echo -e "\n[Program exited with SUCCESS status]"
else
  echo -e "\n[Program exited with FAILURE status]"
fi

