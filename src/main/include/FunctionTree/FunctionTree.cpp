
#include <llvm/IR/Instructions.h>
#include "FunctionTree.h"

//Constructor
FunctionTree::FunctionTree(llvm::Function *func) {
    //Set the properties
    this->func = func;
    this->name = func->getName().str();
}

//Construct a Functiontree from a given function
FunctionTree* FunctionTree::construct(llvm::Function *func) {
    //Init the tree
    auto *FT = new FunctionTree(func);
    //Get the called functions, so we can generate the subtrees
    auto calls = FT->getCalledFunctions();

    //If we call functions, we are not in a leaf...
    if(!calls.empty()){
        //Iterate over the calls
        for (auto &F : calls) {
            //If we have a function that isn't the function we started at...
            if(F != FT->func){
                //Call construct recursively, as there might be further calls in the called functions
                auto sFT = construct(F);
                //Add the constructed tree to the subtrees list
                FT->subtrees.push_back(sFT);
            }
        }
    }

    //Return the constructed FunctionTree
    return FT;
}

//Calculate the functions called from the Function saved in this FunctionTree
std::vector<llvm::Function *> FunctionTree::getCalledFunctions() {
    //Init the vector
    std::vector<llvm::Function *> funcs;

    //Iterate over the BasicBlocks of the function
    for (auto &BB : *this->func) {
        //Iterate over the instructions of the function
        for( auto &I : BB){
            //If we find an instruction, that is a call-instruction
            if(llvm::isa<llvm::CallInst>( I )){
                auto calleeInst = llvm::cast<llvm::CallInst>(&I);
                //Get the called function
                auto *cf = calleeInst->getCalledFunction();
                //Add the function to the list
                funcs.push_back(cf);
            }
        }
    }

    //Return the calculated list
    return funcs;
}

//Print the tree in pre-order
void FunctionTree::printPreorder() {
    if(this->subtrees.empty()){
        llvm::outs() << "------------Leaf-----------\n";
        llvm::outs() << "Node " << this->func->getName() << "\n";
    }else{
        llvm::outs() << "============================================\n";
        for (auto sft : this->subtrees) {
            sft->printPreorder();
        }
        llvm::outs() << "------------Node-----------\n";
        llvm::outs() << "Node " << this->func->getName() << "\n";
        llvm::outs() << "============================================\n\n\n\n";
    }

}

//Get the tree as list in pre-order
std::vector<llvm::Function *> FunctionTree::getPreOrderVector() {
    //init the list
    std::vector<llvm::Function *> list;

    //Test if we are in a leaf
    if(this->subtrees.empty()){
        //If we are in a leaf, add this function to the list. No further recursion will be taken
        list.push_back(this->func);
    }else{
        //If we have subTrees, iterate over them
        for (auto sft : this->subtrees) {
            //Recursivly call this method on the current subtree
            auto sublist = sft->getPreOrderVector();

            //Check if the function is already in the list, if so don't add it to the list
            for(auto F : sublist){
                if(std::find(list.begin(), list.end(), F) == list.end()){
                    list.push_back(F);
                }
            }
        }
        //Add the current function to the list
        list.push_back(this->func);
    }

    //Return the list
    return list;
}


