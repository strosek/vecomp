#!/bin/bash

# Script to execute a given test

if [ -f errores.out ]; then
  rm errores.out
fi

../binaries/vecomp $1 2> errores.out

if [ $? -eq 0 ]; then
  echo -e "\n[Program exited with SUCCESS status]"
else
  echo -e "\n[Program exited with FAILURE status]"
fi

