#include <llvm/Passes/PassBuilder.h>
#include <llvm/Analysis/RegionInfo.h>

#include <utility>
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "../../include/LLVM-Handler/LLVMHandler.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Analysis/PostDominators.h"
#include "../../include/JSON-Handler/JSONHandler.h"
#include "../../include/ProgramGraph/ProgramGraph.h"
#include "../../include/FunctionTree/FunctionTree.h"
#include "../../include/LLVM-Handler//EnergyFunction.h"


llvm::cl::opt<std::string> energyModelPath("model", llvm::cl::desc("Energymodel as JSON"), llvm::cl::value_desc("filepath to .json file"));
llvm::cl::opt<std::string> modeParameter("mode", llvm::cl::desc("Mode the analysis runs on"), llvm::cl::value_desc("Please choose out of the options program/function"));
llvm::cl::opt<std::string> formatParameter("format", llvm::cl::desc("Format to print as result"), llvm::cl::value_desc("Please choose out of the options json/plain"));
llvm::cl::opt<std::string> analysisStrategyParameter("strategy", llvm::cl::desc("The strategy to analyze"), llvm::cl::value_desc("Please choose out of the options worst/average/best"));
llvm::cl::opt<std::string> loopboundParameter("loopbound", llvm::cl::desc("A value to over-approximate loops, which upper bound can't be calculated"), llvm::cl::value_desc("Please provide a positive integer value"));


struct Energy : llvm::PassInfoMixin<Energy> {
    Json::Value energyJson;
    std::string mode;
    std::string format;
    std::string strategy;
    std::string loopbound;

    /**
     * Constructor to run, when called from a method
     * @param filename Path to the .json file containing the energymodel
     * @param mode Mode to run the analysis on
     * @param format Outputformat
     * @param strategy Strategy to analyze the program with
     * @param loopbound Upper bound of loops that can't be analyzed
     */
    explicit Energy(const std::string& filename, std::string mode, std::string format, std::string strategy, std::string loopbound){
        if( llvm::sys::fs::exists( filename ) && !llvm::sys::fs::is_directory( filename ) ){
            //Create a JSONHandler object and read in the energypath
            this->energyJson = JSONHandler::read( filename )["profile"];
            this->mode = std::move(mode);
            this->format = std::move(format);
            this->strategy = std::move(strategy);
            this->loopbound = std::move(loopbound);
        }
    }

    /**
     * Constructor called by the passmanager
     */
    Energy(){
        if( llvm::sys::fs::exists( energyModelPath ) && !llvm::sys::fs::is_directory( energyModelPath ) ){
            //Create a JSONHandler object and read in the energypath
            this->energyJson = JSONHandler::read( energyModelPath.c_str() )["profile"];
            this->mode = modeParameter.c_str();
            this->format = formatParameter.c_str();
            this->strategy = analysisStrategyParameter.c_str();
            this->loopbound = loopboundParameter.c_str();
        }
    }

    /**
     * Construct the JSON-Object from which we will draw all necessary information for the output
     * @param handler LLVMHandler so we can get the functionQueue
     * @return Retuns a JSON-Object containing all needed information for the output
     */
    static Json::Value constructOutputObject(LLVMHandler &handler){
        Json::Value outputObject;

        for (const auto& [key, value] : handler.funcmap){
            auto energyFunction = value;
            Json::Value functionObject;
            functionObject["name"] = energyFunction->func->getName().str();
            functionObject["energy"] = energyFunction->energy;
            functionObject["numberOfBasicBlocks"] = energyFunction->func->getBasicBlockList().size();
            functionObject["numberOfInstructions"] = energyFunction->func->getInstructionCount();
            functionObject["averageEnergyPerBlock"] = energyFunction->energy / (double) energyFunction->func->getBasicBlockList().size();
            functionObject["averageEnergyPerInstruction"] = energyFunction->energy / (double) energyFunction->func->getInstructionCount();

            outputObject[energyFunction->func->getName().str()] = functionObject;
        }

        return outputObject;
    }

    /**
     * Prints the provided JSON-Object as stylized json string
     * @param outputObject JSON-Object containing information about the analysis
     */
    static void outputMetricsJSON(Json::Value outputObject){

        llvm::outs() << outputObject.toStyledString() << "\n";
    }

    /**
     * Print the provided JSON-Object as string
     * @param outputObject JSON-Object containing information about the analysis
     */
    static void outputMetricsPlain(const Json::Value& outputObject){

        for(auto functionObject : outputObject){
            llvm::outs() << "\n";
            llvm::outs() << "Function " << functionObject["name"].asString() << "\n";
            llvm::outs() << "======================================================================" << "\n";
            llvm::outs() << "Estimated energy consumption: " << functionObject["energy"].asDouble()  << " µJ\n";
            llvm::outs() << "Number of basic blocks: " << functionObject["numberOfBasicBlocks"].asDouble()  << "\n";
            llvm::outs() << "Number of instruction: " << functionObject["numberOfInstructions"].asDouble()  << "\n";
            llvm::outs() << "Ø energy per block: " << functionObject["averageEnergyPerBlock"].asDouble()  << " µJ\n";
            llvm::outs() << "Ø energy per instruction: " << functionObject["averageEnergyPerInstruction"].asDouble() << " µJ\n";
            llvm::outs() << "======================================================================" << "\n";
            llvm::outs() << "\n";
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
    static ProgramGraph* constructProgramRepresentation(llvm::Function *function, LLVMHandler *handler, llvm::FunctionAnalysisManager *FAM, AnalysisStrategy::Strategy analysisStrategy){
        auto* domtree = new llvm::DominatorTree();
        domtree->recalculate(*function);

        auto &loopAnalysis = FAM->getResult<llvm::LoopAnalysis>(*function);
        auto &scalarEvolution = FAM->getResult<llvm::ScalarEvolutionAnalysis>(*function);


        //Init a vector of references to BasicBlocks for all BBs in the function
        std::vector<llvm::BasicBlock *> functionBlocks;
        for(auto &blocks : *function){
            functionBlocks.push_back(&blocks);
        }

        //Create the ProgramGraph for the BBs present in the current function
        ProgramGraph *programGraph = ProgramGraph::construct(functionBlocks, function, analysisStrategy);

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
                LoopNode *loopNode = LoopNode::construct(&LT, programGraph, function, analysisStrategy);
                //Replace the blocks used by loop in the previous created ProgramGraph
                programGraph->replaceNodesWithLoopNode(topLoop->getBlocksVector(), loopNode);
            }


            energyCalculation(programGraph, handler, function);


            return programGraph;

        }else{

            energyCalculation(programGraph, handler, function);

            return programGraph;
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
    void analysisRunner(llvm::Module &module, llvm::ModuleAnalysisManager &MAM, AnalysisStrategy::Strategy analysisStrategy, int maxiterations) const{
        //Get the FunctionAnalysisManager from the ModuleAnalysisManager
        auto &functionAnalysisManager = MAM.getResult<llvm::FunctionAnalysisManagerModuleProxy>(module).getManager();

        //If a model was provided
        if(this->energyJson){
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
            LLVMHandler handler = LLVMHandler( this->energyJson, maxiterations);

            //Check with analysis-mode was provided
            if(this->mode == "program"){
                //If the user requested the program-mode we have to consider function calls and their energy-usage

                std::vector<llvm::StringRef> names;
                for(auto function : functionTree->getPreOrderVector()){
                    names.push_back(function->getName());
                }


                for(auto function : functionTree->getPreOrderVector()){
                    //Construct a new EnergyFunction to the current function
                    auto newFuntion = new EnergyFunction(function);

                    //Add the EnergyFunction to the queue
                    //handler.funcqueue.push_back(newFuntion);
                    handler.funcmap[function->getName().str()] = newFuntion;
                    auto energyFunction = handler.funcmap.at(function->getName().str());

                    //Check if the current function is external. Analysis of external functions, that only were declared, will result in an infinite loop
                    if(!function->isDeclarationForLinker()){
                        //Calculate the energy
                        constructProgramRepresentation(function, &handler, &functionAnalysisManager, analysisStrategy);
                    }

                }
            }else if(this->mode == "function"){
                //If the user requested the function-mode, calls only add their own energy-usage without the energy of the called function
                //Iterate over the list of functions
                for(auto &function : *funcList){
                    //Check if the current function is external. Analysis of external functions, that only were declared, will result in an infinite loop
                    if(!function.isDeclarationForLinker()){
                        //Calculate the energy
                        constructProgramRepresentation(&function, &handler, &functionAnalysisManager, analysisStrategy);


                    }
                }
            }else{
                llvm::errs() << "Please specify the mode the pass should run on:\n\t-mode program analyzes the program starting in the main function\n\t-mode function analyzes all functions, without respect to calls" << "\n";
            }



            //Construct the output
            Json::Value output = constructOutputObject(handler);

            //Check which output-format the user requested
            if(format == "json"){
                outputMetricsJSON(output);
            }else if(format == "plain"){
                outputMetricsPlain(output);
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
    llvm::PreservedAnalyses run(llvm::Module &module, llvm::ModuleAnalysisManager &moduleAnalysisManager) const {

        //Try to get the requested loopbound value
        try {
            //Get the int-value from the provided string
            int maxiterations = std::stoi(this->loopbound);

            //Check whether the value is greater than 0
            if(maxiterations >= 0){
                //Check the analysis-strategy the user requestet
                if(strcmp(this->strategy.c_str(), "best") == 0){
                    analysisRunner(module, moduleAnalysisManager, AnalysisStrategy::BESTCASE, maxiterations);
                }else if(strcmp(this->strategy.c_str(), "worst") == 0){
                    analysisRunner(module, moduleAnalysisManager, AnalysisStrategy::WORSTCASE, maxiterations);
                }else if(strcmp(this->strategy.c_str(), "average") == 0){
                    analysisRunner(module, moduleAnalysisManager, AnalysisStrategy::AVERAGECASE, maxiterations);
                }else{
                    llvm::errs() << "Please provide a valid analysis strategy: best/worst/average" << "\n";
                }
            }else{
                throw std::invalid_argument("Loopbound can't be negative");
            }

        }catch(std::invalid_argument &I){
            llvm::errs() << "Please provide a positive integer for the loopbound" << "\n";
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