#ifndef BA_LOOPTREE_H
#define BA_LOOPTREE_H

#include <vector>
#include <llvm/Analysis/LoopInfo.h>
#include "../LLVM-Handler/LLVMHandler.h"

/**
 * LoopTree - A recursive datastructure to handle encapsulated loops
 */
class LoopTree {
public:
    /**
     * Member to store the loop from which the treenode extends to the subtrees containing the subloops
     */
    llvm::Loop * mainloop;

    /**
     * Vector storing all subtrees extending from this treenode
     */
    std::vector<LoopTree *> subTrees;

    /**
     * The basic blocks of the loop excluding the blocks of the subloops
     */
    std::vector<llvm::BasicBlock *> blocks;

    /**
     * A LLVMHandler object, so we can reason about the energy consumption of the loops
     */
    LLVMHandler* handler;

    /**
     * Constructor building the recursive datastructure
     * @param main The loop from which the tree should be builded
     * @param subloops The loops contained in the mainloop
     * @param handler A LLVMHandler to handle calculations on the LLVM IR
     */
    LoopTree(llvm::Loop *main, std::vector<llvm::Loop *> subloops, LLVMHandler *handler);

    /**
     * Calculate the energy over this node, combined with the energy of the subloops
     * @return Returns the energy as double value
     */
    double calcEnergy();

    /**
     * Prints this node in preorder
     */
    void printPreOrder();

    std::vector<llvm::BasicBlock *> getLatches();

/**
 * The over approximated iterations of the loop contained in this node
 */
long iterations;
private:

    /**
     * Method for calculating the difference of all blocks present in the loop and the subloops
     * @return Returns the calculated blocks as vector of pointers
     */
    std::vector<llvm::BasicBlock *> calcBlocks();

    /**
     * Simple method to check if the current LoopTree is a leaf and has now subloops
     * @return Returns true if the current LoopTree is a leaf, false otherwise
     */
    bool isLeaf();

};


#endif //BA_LOOPTREE_H