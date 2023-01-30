#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "The script expects exactly one parameter! The parameter has to be a valid .ll file"
else
  filename="$(basename "$1" .ll)"
  echo "Compiling the file $1 downto binary..."

  echo "Generating the bytcodefile $filename.bc"
  llvm-as "$1"
  echo "Generating the objectfile $filename.o"
  llc -filetype=obj "src/$filename.bc"
  echo "Generating the bynary compiled/$filename"
  clang "src/$filename.o" -o "compiled/$filename"
fi
