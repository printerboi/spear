#include <llvm/Passes/PassBuilder.h>
#include <llvm/Analysis/RegionInfo.h>

#include <utility>
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "../../include/LLVM-Handler/LLVMHandler.h"
#include "../../include/JSON-Handler/JSONHandler.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Analysis/PostDominators.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;


#include "../../include/ProgramGraph/ProgramGraph.h"
#include "../../include/FunctionTree/FunctionTree.h"
#include "../../include/LLVM-Handler//EnergyFunction.h"
#include "../../include/CLIHandler/CLIOptions.h"


llvm::cl::opt<std::string> energyModelPath("profile", llvm::cl::desc("Energymodel as JSON"), llvm::cl::value_desc("filepath to .json file"));
llvm::cl::opt<std::string> modeParameter("mode", llvm::cl::desc("Mode the analysis runs on"), llvm::cl::value_desc("Please choose out of the options program/function"));
llvm::cl::opt<std::string> formatParameter("format", llvm::cl::desc("Format to print as result"), llvm::cl::value_desc("Please choose out of the options json/plain"));
llvm::cl::opt<std::string> analysisStrategyParameter("strategy", llvm::cl::desc("The strategy to analyze"), llvm::cl::value_desc("Please choose out of the options worst/average/best"));
llvm::cl::opt<std::string> loopboundParameter("loopbound", llvm::cl::desc("A value to over-approximate loops, which upper bound can't be calculated"), llvm::cl::value_desc("Please provide a positive integer value"));
llvm::cl::opt<std::string> deepCallsParameter("withcalls", llvm::cl::desc("If flag is provided calls will contribute their own energy usage and the usage of the called function to the result"), llvm::cl::value_desc(""));


struct Energy : llvm::PassInfoMixin<Energy> {
    json energyJson;
    Mode mode;
    Format format;
    Strategy strategy;
    int loopbound;
    bool deepCallsEnabled;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> stopwatch_start;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> stopwatch_end;

    /**
     * Constructor to run, when called from a method
     * @param filename Path to the .json file containing the energymodel
     * @param mode Mode to run the analysis on
     * @param format Outputformat
     * @param strategy Strategy to analyze the program with
     * @param loopbound Upper bound of loops that can't be analyzed
     */
    explicit Energy(const std::string& filename, Mode mode, Format format, Strategy strategy, int loopbound, DeepCalls deepCalls){
        if( llvm::sys::fs::exists( filename ) && !llvm::sys::fs::is_directory( filename ) ){
            //Create a JSONHandler object and read in the energypath
            this->energyJson = JSONHandler::read(filename)["profile"];
            std::cout << this->energyJson.dump() << std::endl;

            this->mode = mode;
            this->format = format;
            this->strategy = strategy;
            this->loopbound = loopbound;
            this->stopwatch_start = std::chrono::high_resolution_clock::now();
            this->deepCallsEnabled = (deepCalls == DeepCalls::ENABLED);
        }
    }

    /**
     * Constructor called by the passmanager
     */
    Energy(){
        if( llvm::sys::fs::exists( energyModelPath ) && !llvm::sys::fs::is_directory( energyModelPath ) ){
            //Create a JSONHandler object and read in the energypath
            this->energyJson = JSONHandler::read(energyModelPath.c_str())["profile"];
            this->mode = CLIOptions::strToMode(modeParameter.c_str());
            this->format = CLIOptions::strToFormat(formatParameter.c_str());
            this->strategy = CLIOptions::strToStrategy(analysisStrategyParameter.c_str());
            this->strategy = CLIOptions::strToStrategy(analysisStrategyParameter.c_str());
            this->deepCallsEnabled = !analysisStrategyParameter.empty();

            //Try to get the requested loopbound value
            try {
                this->loopbound = std::stoi(loopboundParameter.c_str());

                if(this->loopbound < 0){
                    throw std::invalid_argument("Loopbound can't be negative");
                }
            }catch(std::invalid_argument &I){
                llvm::errs() << "Please provide a positive integer for the loopbound" << "\n";
                return;
            }


            this->stopwatch_start = std::chrono::high_resolution_clock::now();
        }
    }

    /**
     * Construct the JSON-Object from which we will draw all necessary information for the output
     * @param handler LLVMHandler so we can get the functionQueue
     * @return Retuns a JSON-Object containing all needed information for the output
     */
    static json constructOutputObject(EnergyFunction funcpool[], int numberOfFuncs, Mode mode){
        json outputObject = json::object();
        outputObject["functions"] = json::array();

        if(mode == Mode::PROGRAM){
            for (int i=0; i < numberOfFuncs; i++){
                auto energyFunction = &funcpool[i];
                json functionObject = json::object();
                functionObject["name"] = energyFunction->func->getName().str();
                functionObject["energy"] = energyFunction->energy;
                functionObject["numberOfBasicBlocks"] = energyFunction->func->size();
                functionObject["numberOfInstructions"] = energyFunction->func->getInstructionCount();

                if(!energyFunction->func->empty()){
                    functionObject["averageEnergyPerBlock"] = energyFunction->energy / (double) energyFunction->func->size();
                } else {
                    functionObject["averageEnergyPerBlock"] = 0;
                }

                if(energyFunction->func->getInstructionCount() > 0){
                    functionObject["averageEnergyPerInstruction"] = energyFunction->energy / (double) energyFunction->func->getInstructionCount();
                } else {
                    functionObject["averageEnergyPerInstruction"] = 0;
                }

                outputObject["functions"][i] = functionObject;
            }
        }else if(mode == Mode::BLOCK){
            for (int i=0; i < numberOfFuncs; i++){
                auto energyFunction = &funcpool[i];
                json functionObject = json::object();
                functionObject["name"] = energyFunction->func->getName().str();
                functionObject["nodes"] = json::array();

                if(energyFunction->programGraph != nullptr){
                    std::vector<Node *> nodelist = energyFunction->programGraph->getNodes();
                    if(!nodelist.empty()){
                        for(int j=0; j < nodelist.size(); j++){
                            json nodeObject = json::object();
                            Node* Node = nodelist[j];

                            if(Node->block){
                                nodeObject["name"] = Node->block->getName().str();
                                if(Node->energy > 0){
                                    nodeObject["energy"] = Node->energy;
                                    functionObject["nodes"][j] = nodeObject;
                                }else{
                                    std::cout << "";
                                }


                            }
                        }
                    }
                }

                outputObject["functions"][i] = functionObject;
            }
        }

        return outputObject;
    }

    /**
     * Prints the provided JSON-Object as stylized json string
     * @param outputObject JSON-Object containing information about the analysis
     */
    static void outputMetricsJSON(json outputObject, Mode mode){

        llvm::outs() << outputObject.dump(4) << "\n";
    }

    /**
     * Print the provided JSON-Object as string
     * @param outputObject JSON-Object containing information about the analysis
     */
    static void outputMetricsPlain(json& outputObject, Mode mode){

        if(mode == Mode::PROGRAM){
            auto timeused = outputObject["duration"].get<double>();
            outputObject.erase("duration");

            for(auto functionObject : outputObject){
                if(functionObject.contains("name")){
                    //llvm::errs() << functionObject.toStyledString() << "\n\n\n";
                    llvm::outs() << "\n";
                    llvm::outs() << "Function " << functionObject["name"].dump() << "\n";
                    llvm::outs() << "======================================================================" << "\n";
                    llvm::outs() << "Estimated energy consumption: " << functionObject["energy"].get<double>()  << " J\n";
                    llvm::outs() << "Number of basic blocks: " << functionObject["numberOfBasicBlocks"].get<int>()  << "\n";
                    llvm::outs() << "Number of instruction: " << functionObject["numberOfInstructions"].get<int>() << "\n";
                    llvm::outs() << "Ø energy per block: " << functionObject["averageEnergyPerBlock"].get<double>()  << " J\n";
                    llvm::outs() << "Ø energy per instruction: " << functionObject["averageEnergyPerInstruction"].get<double>() << " J\n";
                    llvm::outs() << "======================================================================" << "\n";
                    llvm::outs() << "\n";
                }
            }
            llvm::outs() << "The Analysis took: " << timeused << " s\n";
        }else if(mode == Mode::BLOCK){

        }else{
            llvm::errs() << "Please specify the mode the pass should run on:\n\t-mode program analyzes the program starting in the main function\n\t-mode function analyzes all functions, without respect to calls" << "\n";
        }
    }

    /**
     * Calculates ProgramGraph-representation of a function
     * @param function Function to construct the graph for
     * @param handler A LLVMHandler containing the energy-Model
     * @param FAM A llvm::FunctionAnalysisManager
     * @param analysisStrategy The strategy to analyze the function with
     * @return Returns the calculated ProgramGraph
     */
    static void constructProgramRepresentation(ProgramGraph* pGraph, EnergyFunction *energyFunc, LLVMHandler *handler, llvm::FunctionAnalysisManager *FAM, AnalysisStrategy::Strategy analysisStrategy){
        auto* domtree = new llvm::DominatorTree();
        llvm::Function* function = energyFunc->func;

        domtree->recalculate(*function);

        auto &loopAnalysis = FAM->getResult<llvm::LoopAnalysis>(*function);
        auto &scalarEvolution = FAM->getResult<llvm::ScalarEvolutionAnalysis>(*function);


        //Init a vector of references to BasicBlocks for all BBs in the function
        std::vector<llvm::BasicBlock *> functionBlocks;
        for(auto &blocks : *function){
            functionBlocks.push_back(&blocks);
        }

        //Create the ProgramGraph for the BBs present in the current function
        ProgramGraph::construct(pGraph, functionBlocks, analysisStrategy);

        //Get the vector of Top-Level loops present in the program
        auto loops = loopAnalysis.getTopLevelLoops();

        //We need to distinguish if the function contains loops
        if(!loops.empty()){
            //If the function contains loops

            //Iterate over the top-level loops
            for (auto liiter = loops.begin(); liiter < loops.end(); ++liiter) {
                //Get the loop, the iterator points to
                auto topLoop= *liiter;

                //Construct the LoopTree from the Information of the current top-level loop
                LoopTree LT = LoopTree(topLoop, topLoop->getSubLoops(), handler, &scalarEvolution);

                //Construct a LoopNode for the current loop
                LoopNode *loopNode = LoopNode::construct(&LT, pGraph, analysisStrategy);
                //Replace the blocks used by loop in the previous created ProgramGraph
                pGraph->replaceNodesWithLoopNode(topLoop->getBlocksVector(), loopNode);
            }


            //energyCalculation(pGraph, handler, function);
            energyFunc->energy = pGraph->getEnergy(handler);

        }else{

            //energyCalculation(pGraph, handler, function);
            energyFunc->energy = pGraph->getEnergy(handler);
        }

    }

    static void energyCalculation(ProgramGraph *programGraph, LLVMHandler *handler, llvm::Function *function){
        auto energyFunction = handler->funcmap[function->getName().str()];
        energyFunction->energy = programGraph->getEnergy(handler);
    }

    /**
     * Function to run the analysis on a given module
     * @param module LLVM::Module to run the analysis on
     * @param MAM llvm::ModuleAnalysisManager
     * @param analysisStrategy Strategy to analyze the module with
     * @param maxiterations Upper bound of loops
     */
    void analysisRunner(llvm::Module &module, llvm::ModuleAnalysisManager &MAM, AnalysisStrategy::Strategy analysisStrategy, int maxiterations) {
        //Get the FunctionAnalysisManager from the ModuleAnalysisManager
        auto &functionAnalysisManager = MAM.getResult<llvm::FunctionAnalysisManagerModuleProxy>(module).getManager();
        static ProgramGraph* pGraph = new ProgramGraph();


        //If a model was provided
        if( this->energyJson.contains("Division") && this->energyJson.contains("Memory") ){
            //Get the functions from the module
            auto funcList = &module.getFunctionList();
            FunctionTree * functionTree;

            //Construct the functionTrees to the functions of the module
            for(auto &function : *funcList){
                if(function.getName() == "main"){
                    auto mainFunctionTree = FunctionTree::construct(&function);
                    functionTree = (mainFunctionTree);
                }
            }

            //Init the LLVMHandler with the given model and the upper bound for unbounded loops
            LLVMHandler handler = LLVMHandler(this->energyJson, maxiterations, deepCallsEnabled);

            std::vector<llvm::StringRef> names;
            for (auto function: functionTree->getPreOrderVector()) {
                names.push_back(function->getName());
            }

            EnergyFunction funcPool[functionTree->getPreOrderVector().size()];


            for (int i=0; i < functionTree->getPreOrderVector().size(); i++) {
                //Construct a new EnergyFunction to the current function
                //auto newFuntion = new EnergyFunction(function);
                llvm::Function * function = functionTree->getPreOrderVector()[i];

                //Add the EnergyFunction to the queue
                //handler.funcqueue.push_back(newFuntion);
                //auto energyFunction = handler.funcmap.at(function->getName().str());

                funcPool[i].func = function;

                //Check if the current function is external. Analysis of external functions, that only were declared, will result in an infinite loop
                if (!function->isDeclarationForLinker()) {
                    //Calculate the energy
                    constructProgramRepresentation(pGraph, &funcPool[i], &handler, &functionAnalysisManager, analysisStrategy);
                    funcPool[i].programGraph = pGraph;
                }else{
                    funcPool[i].programGraph = nullptr;
                }

            }

            //Construct the output
            json output = constructOutputObject(funcPool, functionTree->getPreOrderVector().size(),  this->mode);

            this->stopwatch_end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> ms_double = this->stopwatch_end - this->stopwatch_start;

            output["duration"] = ms_double.count()/1000;

            if(format == Format::JSON){
                outputMetricsJSON(output, this->mode);
            }else if(format == Format::PLAIN){
                outputMetricsPlain(output, this->mode);
            }else{
                llvm::errs() << "Please provide a valid output format: plain/JSON" << "\n";
            }


        }else{
            llvm::errs() << "Please provide an energyfile with -m <path to the energy.json>" << "\n";
        }
    }

    /**
     * Main runner of the energy pass. The pass will apply module-wise.
     * @param module Reference to a Module
     * @param moduleAnalysisManager Reference to a ModuleAnalysisManager
     */
    llvm::PreservedAnalyses run(llvm::Module &module, llvm::ModuleAnalysisManager &moduleAnalysisManager) {

        //Get the int-value from the provided string
        int maxiterations = this->loopbound;


        //Check the analysis-strategy the user requestet
        if(this->strategy == Strategy::BEST){
            analysisRunner(module, moduleAnalysisManager, AnalysisStrategy::BESTCASE, maxiterations);
        }else if(this->strategy == Strategy::WORST){
            analysisRunner(module, moduleAnalysisManager, AnalysisStrategy::WORSTCASE, maxiterations);
        }else if(this->strategy == Strategy::AVERAGE){
            analysisRunner(module, moduleAnalysisManager, AnalysisStrategy::AVERAGECASE, maxiterations);
        }else{
            llvm::errs() << "Please provide a valid analysis strategy: best/worst/average" << "\n";
        }

        return llvm::PreservedAnalyses::all();

    }

    static bool isRequired() { return true; }
};



/**
 * Method for providing some basic information about the pass
 */
llvm::PassPluginLibraryInfo getEnergyPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "Energy", LLVM_VERSION_STRING,
            [](llvm::PassBuilder &PB) {
                PB.registerPipelineParsingCallback(
                        [](llvm::StringRef Name, llvm::ModulePassManager &modulePassManager,
                           llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                            if (Name == "energy") {
                                modulePassManager.addPass(Energy());
                                return true;
                            }
                            return false;
                        });
            }};
}

//Register the pass in llvm, so it is useable with opt
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return getEnergyPluginInfo();
}