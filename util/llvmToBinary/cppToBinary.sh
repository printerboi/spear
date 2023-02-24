#!/bin/bash



function compileFile() {
  filename=$(basename -- "$1")
  path=$(dirname -- "$1")
  extension="${filename##*.}"
  filename="${filename%.*}"



  mkdir -p "$path/compiled"

  echo "Generating the llvm-IR"
  clang++ -O0 -Xclang -disable-O0-optnone -fno-discard-value-names -S -emit-llvm -o "$path/compiled/$filename.ll" "$path/$filename.cpp"

  echo "Compiling the file $path/compiled/$filename.ll downto binary..."

  echo "Generating the bytcodefile $filename.bc"
  llvm-as "$path/compiled/$filename.ll"
  echo "Generating the objectfile $filename.o"
  llc -filetype=obj "$path/compiled/$filename.bc"
  echo "Generating the binary $path/compiled/$filename"

  clang++ -no-pie "$path/compiled/$filename.o" -o "$path/compiled/$filename"

  rm "$path/compiled/$filename.bc"
  rm "$path/compiled/$filename.o"
}


for f in $1/*.cpp
do
  compileFile "$f"
done
