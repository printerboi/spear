# SPEAR

Welcome to SPEAR, the 
**S**tatic **P**redictive **E**nergy **A**nalysis Tool based on **R**APL.

The tool will be developed for my bachelor-thesis
**"Predictive, static energy consumption analysis based on experimentally determined energy models"**

## Introduction

Modern computers, especially in high computing applications require a lot of energy.
Data centers, which handle all of our modern Cloud-Infrastructure, are trying to save every last bit of energy to reduce costs.
Even though energy seems to be a problematic factor regarding cost and environment, most
developers are not well aware about the energy-consumption of their software. Either trough missing information about their
used architecture or through the abstraction their used language implements.

To work towards filling this knowledge-gap, this bachelor-thesis will try to develop a tool
for static analysis of LLVM-IR Code which will get populated with energy-consumption profiling
from the Intel RAPL Interface.

## Running the profiler

Before running an analysis with spear, you have to profile your device with the built-in profiler.
You have to compile the profile-scripts for your machine. We provided a shell-script to simplify this step.
Run the script `irToBinary.sh` in the `profile/` folder

After the compilation run spear with the flag `-p` and provide the number of iterations and repetitions.
E.g.:

```
spear -p 100 100
```

Please choose the parameters for iterations and repetitions accordingly to your setup.

## Running an Analysis

We implemented two ways to run the analysis. Before the analysis make sure, you compiled your program with the following
parameters:

```
clang++ -O0 -Xclang -disable-O0-optnone -fno-discard-value-names -S -emit-llvm <file>
```
Where `<file>` is the source-file you want to analyze.
Please compile your source-file with this command,
otherwise the behaviour of spear will be undefined or the analysis will crash.

### 1) Using Spear

Spear provided a custom flag `-a`, which expects the following parameters:

```
spear -a <modelpath> --mode <mode> --format <format> --strategy <strategy> --loopbound <loopbound> <llvmirpath>
```

`<modelpath>`: Path to a model calculated by spear with the `-p` flag

`<mode>`: Mode the analysis should run on. Choose between the following options
- `function` - Analyses every function by itself. Takes no respect to calls
- `program` - Analyses the whole program with respect to calls.

`<format>`: Outputformat to print after the calculation. Choose between the following options
- `json` - Json-Format
- `plain` - Human-readable plaintext

`<strategy>`: Analysis-strategy to calculate. Choose between the following options
- `worst` - Worstcase-analysis. Will take path with the most energy consumption, when calculating an if-condition
- `best` - Bestcase-analysis. Will take path with the least energy consumption, when calculating an if-condition
- `average` - Chooses a random path, when calculating an if-condition

`<loopbound>`: A positive integer defining a value, loops will be over-approximated, if spear can't interfere the iteration count from a loop. E.g. in a while-loop

`<llvmirpath>` Path to compiled llvm-ir file, the analysis should run on

### 2) Using the LLVM pass

We provided a custom LLVM pass to run with the `opt` tool. To run the analysis, use the following preset:

```
opt -disable-output 
    -load-pass-plugin <libpath>
    --passes="function(instnamer,mem2reg,loop-simplify,loop-rotate),energy"
    --model <modelpath>
    --mode <mode> 
    --format <format> 
    --strategy <strategy> 
    --loopbound <loopbound>
    <llvmirpath>
```

`<libpath>`: Path to the compiled energy-library `Energy.so`

`<modelpath>`: Path to a model calculated by spear with the `-p` flag

`<mode>`: Mode the analysis should run on. Choose between the following options
- `function` - Analyses every function by itself. Takes no respect to calls
- `program` - Analyses the whole program with respect to calls.

`<format>`: Outputformat to print after the calculation. Choose between the following options
- `json` - Json-Format
- `plain` - Human-readable plaintext

`<strategy>`: Analysis-strategy to calculate. Choose between the following options
- `worst` - Worstcase-analysis. Will take path with the most energy consumption, when calculating an if-condition
- `best` - Bestcase-analysis. Will take path with the least energy consumption, when calculating an if-condition
- `average` - Chooses a random path, when calculating an if-condition

`<loopbound>`: A positive integer defining a value, loops will be over-approximated, if spear can't interfere the iteration count from a loop. E.g. in a while-loop

`<llvmirpath>` Path to compiled llvm-ir file, the analysis should run on