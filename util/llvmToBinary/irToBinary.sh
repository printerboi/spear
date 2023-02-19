#!/bin/bash



function compileFile() {
  filename=$(basename -- "$1")
  path=$(dirname -- "$1")
  extension="${filename##*.}"
  filename="${filename%.*}"


  echo "Compiling the file $path/$filename.ll downto binary..."

  echo "Generating the bytcodefile $filename.bc"
  llvm-as "$path/$filename.ll"
  echo "Generating the objectfile $filename.o"
  llc -filetype=obj "$path/$filename.bc"
  echo "Generating the binary $path/compiled/$filename"
  mkdir -p "$path/compiled"
  clang "$path/$filename.o" -o "$path/compiled/$filename"

  rm $path/$filename.bc
  rm $path/$filename.o
}


for f in $1/*.ll
do
  compileFile "$f"
done
