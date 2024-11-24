# SPEAR opt pass

If you want to use SPEAR as pass for llvm opt, use the following explanation:

## Using the LLVM pass

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
    --withCalls <withcalls>
    --program <llvmirpath>
    --forFunction <function>
```

`<libpath>`: Path to the compiled energy-library `Energy.so`

`<profilepath>`: Path to a profile calculated by spear with the `-p` flag

`<mode>`: Mode the analysis should run on. Choose between the following options
- `function` - Analyses every function by itself. Takes no respect to calls
- `program` - Analyses the whole program with respect to calls.
- `block`- Analyses the whole program but will output the nodes of the programgraph for each function. Denotes the nodes with their energy usage.
- `instruction` - Analyses the whole program and will output a JSON representing the programgraph down to the used instructions of each node.
- `graph`- Analyses the program and will print out a DOT graph to stdout.

`<format>`: Outputformat to print after the calculation. Choose between the following options
- `json` - Json-Format
- `plain` - Human-readable plaintext

`<strategy>`: Analysis-strategy to calculate. Choose between the following options
- `worst` - Worst case analysis. During the calculation the paths with the most energy consumption, will be used
- `best` - Best case analysis. During the calculation the paths with the most energy consumption, will be used
- `average` - Average case analysis. Attempts to balance the number of energy efficient and energy inefficient paths

`<withcalls>`: Determines if the analysis should use analyse called functions.

`<loopbound>`: A positive integer defining a value, loops will be over-approximated, if spear can't interfere the iteration count from a loop. E.g. in a while-loop

`<llvmirpath>` Path to compiled llvm-ir file, the analysis should run on

`<function>`: Name of the function to analysed. Can be used if only a single function should be analyzed. (Currently only used for graph generation)