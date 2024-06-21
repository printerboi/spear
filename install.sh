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


# Check the user id of the executing user -> ensures privileged rights
if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi


# Install dependencies
if ! command -v clang-17 &> /dev/null
then
  # Install llvm-17
  mkdir /opt/spear
  wget https://apt.llvm.org/llvm-snapshot.gpg.key -O /opt/spear/apt.llvm.org.asc
  gpg --output /opt/spear/apt.llvm.org.gpg --dearmor /opt/spear/apt.llvm.org.asc
  mv apt.llvm.org.gpg /etc/apt/keyrings/
  wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | gpg --dearmor | sudo tee /etc/apt/keyrings/apt.llvm.org.gpg
  sudo echo "Types: deb\n URIs: https://apt.llvm.org/bookworm/\n Suites: llvm-toolchain-bookworm llvm-toolchain-bookworm-17 llvm-toolchain-bookworm-16\n Components: main\n Signed-By: /etc/apt/keyrings/apt.llvm.org.gpg\n" /etc/apt/sources.list.d/apt.llvm.org.sources
  sudo apt update
  sudo apt install clang-17 lldb-17 lld-17 clangd-17 clang-tools-17
  ln -s /usr/bin/clang-17 /usr/bin/clang
  ln -s /usr/bin/clang++-17 /usr/bin/clang++
  ln -s /usr/bin/llvm-as-17 /usr/bin/llvm-as
  ln -s /usr/bin/llc-17 /usr/bin/llc
  ln -s /usr/bin/llvm-link-17 /usr/bin/llvm-link
  ln -s /usr/bin/llvm-dis-17 /usr/bin/llvm-dis
fi

if ! dpkg -s nlohmann-json3-dev &> /dev/null
then
  # Install nlohnmann-json
  apt install nlohmann-json3-dev
fi

# Install SPEAR
mkdir build
cd build || exit
echo "[1/6] Compiling SPEAR"
cmake ..
make -j"$(nproc)"
echo "[2/6] Copying profiling programs"
mkdir profile
cd profile || exit
cp ../../profile/src/*.ll -R .
echo "[3/6] Compiling profiling programs"

for f in *.ll
do
  compileFile "$f"
done

cd ..

echo "[4/6] Copying spear to /usr/bin"
sudo install spear /usr/bin

echo "[5/6] Copying profile programs to /usr/share"
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

cd ../../../

echo "[6/6] Copying additional assets"
sudo install scale.png /usr/share/spear/scale.png
echo "[DONE] SPEAR was installed successfully!"