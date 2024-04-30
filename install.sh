#!/bin/bash

function compileFile() {
  filename=$(basename -- "$1")
  path="."
  extension="${filename##*.}"
  filename="${filename%.*}"



  mkdir -p "$path/compiled"

  printf "=============%s=============\n" "$filename"
  printf "\t=> Generating the llvm-IR\n"
  printf "\t=> Compiling the file %s.ll downto binary...\n" $path/$filename

  printf "\t=> Generating the bytcodefile %s.bc\n" $filename
  llvm-as "$path/$filename.ll" -o "$path/compiled/$filename.bc"
  printf "\t=> Generating the objectfile %s.o\n" $filename
  llc -O0 -filetype=obj "$path/compiled/$filename.bc"
  printf "\t=> Generating the binary %s/compiled/%s\n" $path $filename

  clang++ -O0 -no-pie "$path/compiled/$filename.o" -o "$path/compiled/$filename"

  printf "\t=> Cleaning up...\n"
  rm "$path/compiled/$filename.bc"
  rm "$path/compiled/$filename.o"
}


mkdir build
cd build || exit
echo "[1/5] Compiling SPEAR"
cmake ..
make -j"$(nproc)"
echo "[2/5] Copying profiling programs"
mkdir profile
cd profile || exit
cp ../../profile/src/*.ll -R .
echo "[3/5] Compiling profiling programs"

for f in *.ll
do
  compileFile "$f"
done

cd ..

echo "[4/5] Copying spear to /usr/bin"
sudo install spear /usr/bin

echo "[5/5] Copying profile programs to /usr/share"
sudo install -d profile /usr/share/spear/profile
cd profile || exit
cd compiled || exit
sudo install call /usr/share/spear/profile/call
sudo install cast /usr/share/spear/profile/cast
sudo install division /usr/share/spear/profile/division
sudo install memoryread /usr/share/spear/profile/memoryread
sudo install memorywrite /usr/share/spear/profile/memorywrite
sudo install programflow /usr/share/spear/profile/programflow
sudo install stdbinary /usr/share/spear/profile/stdbinary