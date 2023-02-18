#!/bin/bash

function compileFile() {
  filename="src/$1.ll"
  echo "Compiling the file $1 downto binary..."

  echo "Generating the bytcodefile $1.bc"
  llvm-as "$filename"
  echo "Generating the objectfile $1.o"
  llc -filetype=obj "src/$1.bc"
  echo "Generating the bynary compiled/$1"
  clang "src/$1.o" -o "compiled/$1"
}

compileFile base
compileFile groupB
compileFile groupD
compileFile groupG
compileFile groupM
