
<p align="center">
  <img src="./misc/logo.png">
</p>

---


Welcome to SPEAR, the 
**S**tatic **P**redictive **E**nergy **A**nalysis Tool based on Intel **R**APL.

The tool will be developed for my bachelor-thesis
**"Predictive, static energy consumption analysis based on experimentally determined energy models"**


## Installation

We added an installation script to automate the building and copying of the application. The script was designed to be run under Debian and uses the apt package manager.
If you want to use the script on a different distribution adapt the installation script accordingly. Alternativly you could build SPEAR using the build commands, see section [building](#building) for further details.

⚠️ The installation script requires you to have elevated rights. Please execute it using sudo. The application will be installed for all users of the machine.

To run the script execute:
```
chmod +x install.sh
sudo ./install.sh
```



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

Run spear with the command `profile` and provide the number of iterations the profile gets averaged over as `<iterations>`.
The parameter `<model>` expects the spear profile programs.
Normally the profile model is found aftert the installation in the folder `/usr/share/spear/profile`

You can compile the profile-scripts for your machine manually. We provided a shell-script to simplify this step.
Run the script `utils/llvmToBinary/irToBinary.sh` and provide the path to the profile-folder e.g `../../profile/src`
as argument to prepare the files. 

The parameter `<savelocation>` specifies the location where the generated profile will be saved
E.g.:

```
spear profile --iterations <iterations>
              --model <model>
              --savelocation <savelocation>
```
Please execute the command above with elevated rights. Otherwise, Spear can not interfere the energy-values from the system,
as the RAPL Interface is limited to elevated rights only.

Choose the value used for averaging with respect to your setup.
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

### Using Spear

Spear provided a custom flag `analyze`, which expects the following parameters:

```
spear analyze
      --profile <profile> 
      --mode <mode> 
      --format <format> 
      --strategy <strategy> 
      --loopbound <loopbound> 
      --withCalls <withcalls>
      --program <llvmirpath>
      --forFunction <function>
```

`<profile>`: Path to a profile calculated by spear with the `-p` flag

`<mode>`: Mode the analysis should run on. Choose between the following options
- `function` - Analyses every function by itself. Takes no respect to calls
- `program` - Analyses the whole program with respect to calls.
- `block`- Analyses the whole program but will output the nodes of the programgraph for each function. Denotes the nodes with their energy usage.
- `instruction` - Analyses the whole program and will output a JSON representing the programgraph down to the used instructions of each node.
- `graph`- Analyses the program and will print out a DOT graph to stdout.

`<format>`: Outputformat to print after the calculation. Choose between the following options
- `json` - json formatted output
- `plain` - Human-readable plaintext

`<strategy>`: Analysis-strategy to calculate. Choose between the following options
- `worst` - Worst case analysis. During the calculation the paths with the most energy consumption, will be used
- `best` - Best case analysis. During the calculation the paths with the most energy consumption, will be used
- `average` - Average case analysis. Attempts to balance the number of energy efficient and energy inefficient paths

`<withcalls>`: Determines if the analysis should use analyse called functions.

`<loopbound>`: A positive integer defining a value, the iterations of loops will be approximated with, if spear can't interfere the iteration count trough llvm. E.g. in a while-loop

`<llvmirpath>` Path to a compiled llvm-ir file, the analysis should run on

`<function>`: Name of the function to analysed. Can be used if only a single function should be analyzed. (Currently only used for graph generation)

## SPEAR-Viewer

SPEAR now has a Visual Studio Code extension called "SPEAR-Viewer" which applies the algorithms directly on C/C++ code in the editor.
Check out the project [here](https://github.com/printerboi/spear-viewer)


## Contribute

Please feel free to open issues in this repository and create merge request if you like. Please respect, that I run this repository as side project and can only spend my time partly on developing SPEAR.

If you encounter a problem an want to create a issue, please describe your system and problem detailed as possible. A detailed explanation on how to reproduce the problem should be provided.

## Licensing

As SPEAR was created with the goal of improving the enviromental impact of software development, the usage, reuse in different projects etc. are free under the GPL-3.0 license. Please see the license supplied within this repository for further details.