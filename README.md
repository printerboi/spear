# SPEAR

Welcome to SPEAR, the 
**S**tatic **P**redictive **E**nergy **A**nalysis Tool based on Intel **R**APL.

The tool will be developed for my bachelor-thesis
**"Predictive, static energy consumption analysis based on experimentally determined energy models"**

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.8061579.svg)](https://doi.org/10.5281/zenodo.8061579)


## Introduction

Modern computers, especially in high computing applications require a lot of energy.
Data centers, which handle all of our modern Cloud-Infrastructure, are trying to save every last bit of energy to reduce costs.
Even though energy seems to be a problematic factor regarding cost and environment, most
developers are not well aware about the energy-consumption of their software. Either trough missing information about their
used architecture or through the abstraction their used language implements.

To work towards filling this knowledge-gap, this bachelor-thesis provides a tool
for static analysis of LLVM-IR Code which will get populated with energy-consumption profiling
from the Intel RAPL Interface.

## Running the profiler

Before running an analysis with spear, you have to profile your device with the built-in profiler.
Therefore, you have to compile the profile-scripts for your machine. We provided a shell-script to simplify this step.
Run the script `utils/llvmToBinary/irToBinary.sh` and provide the path to the profile-folder e.g `../../profile/src`
as argument to prepare the files. 

After the compilation run spear with the flag `-p` and provide the number of iterations the profile gets averaged over.
E.g.:

```
spear -p 10000
```
Please execute the command above with elevated rights. Otherwise, Spear can not interfere the energy-values from the system,
as the RAPL Interface is limited to elevated rights only.

Choose value used for averaging with respect to your setup.
A larger count of iterations will increase the accuracy of the calculated model, but will also increase the time the profiler runs.

## Running an Analysis

We implemented two ways to run the analysis. Before the analysis make sure, you compiled your program with the following
script we provided:

```
util/llvmToBinary/cppToBinary.sh <path>
```

Where `<path>` is the path to your program files. The script will compile all `.cpp` files in the given path
Please compile your source-files with this script,
otherwise the behaviour of spear will be undefined or the analysis will crash.

### 1) Using Spear

Spear provided a custom flag `-a`, which expects the following parameters:

```
spear -a
      --model <profile> 
      --mode <mode> 
      --format <format> 
      --strategy <strategy> 
      --loopbound <loopbound> 
      <llvmirpath>
```

`<profile>`: Path to a profile calculated by spear with the `-p` flag

`<mode>`: Mode the analysis should run on. Choose between the following options
- `function` - Analyses every function by itself. Takes no respect to calls
- `program` - Analyses the whole program with respect to calls.

`<format>`: Outputformat to print after the calculation. Choose between the following options
- `json` - json formatted output
- `plain` - Human-readable plaintext

`<strategy>`: Analysis-strategy to calculate. Choose between the following options
- `worst` - Worst case analysis. During the calculation the paths with the most energy consumption, will be used
- `best` - Best case analysis. During the calculation the paths with the most energy consumption, will be used
- `average` - Average case analysis. Attempts to balance the number of energy efficient and energy inefficient paths

`<loopbound>`: A positive integer defining a value, the iterations of loops will be approximated with, if spear can't interfere the iteration count trough llvm. E.g. in a while-loop

`<llvmirpath>` Path to a compiled llvm-ir file, the analysis should run on

### 2) Using the LLVM pass

We provided a custom LLVM pass to run with the `opt` tool. To run the analysis, use the following preset:

```
opt -disable-output 
    -load-pass-plugin <libpath>
    --passes="function(mem2reg,loop-rotate),energy"
    --profile <profilepath>
    --mode <mode> 
    --format <format> 
    --strategy <strategy> 
    --loopbound <loopbound>
    <llvmirpath>
```

`<libpath>`: Path to the compiled energy-library `Energy.so`

`<profilepath>`: Path to a profile calculated by spear with the `-p` flag

`<mode>`: Mode the analysis should run on. Choose between the following options
- `function` - Analyses every function by itself. Takes no respect to calls
- `program` - Analyses the whole program with respect to calls.

`<format>`: Outputformat to print after the calculation. Choose between the following options
- `json` - Json-Format
- `plain` - Human-readable plaintext

`<strategy>`: Analysis-strategy to calculate. Choose between the following options
- `worst` - Worst case analysis. During the calculation the paths with the most energy consumption, will be used
- `best` - Best case analysis. During the calculation the paths with the most energy consumption, will be used
- `average` - Average case analysis. Attempts to balance the number of energy efficient and energy inefficient paths

`<loopbound>`: A positive integer defining a value, loops will be over-approximated, if spear can't interfere the iteration count from a loop. E.g. in a while-loop

`<llvmirpath>` Path to compiled llvm-ir file, the analysis should run on
