//
// Created by maximiliank on 03.02.23.
//

#ifndef BA_PROGRAMTREE_H
#define BA_PROGRAMTREE_H


#include <vector>
#include <llvm/Analysis/RegionInfo.h>
#include "llvm/IR/BasicBlock.h"
#include "../LoopTree/LoopTree.h"
#include <cfloat>

class ProgramTree;

class Node {
    public:
        ProgramTree *parent;
        std::vector<llvm::BasicBlock *> blocks;

        Node(ProgramTree *parent);

        virtual std::string toString();
        virtual double getEnergy(LLVMHandler *handler);

protected:
    std::vector<Node *> getAdjacentNodes();
};

class Edge {
public:
    Node * start;
    Node * end;

    Edge(Node *start, Node *end);
    std::string toString();
};

class LoopNode : public Node {
public:
    LoopNode(LoopTree *LT, ProgramTree *parent);
    LoopTree *loopTree;
    std::vector<ProgramTree *> subtrees;
    static LoopNode* construct(LoopTree *lptr, ProgramTree *parent);
    bool isLeafNode();
    double getEnergy(LLVMHandler *handler);
    void removeLoopEdgesFromSubtrees();

    std::string toString();
};

class ProgramTree {
    public:
        std::vector<Node *> nodes;
        std::vector<Edge *> edges;

        static ProgramTree* construct(std::vector<llvm::BasicBlock *> blockset);

        void printNodes(LLVMHandler *handler);
        void printEdges();
        Node *findBlock(llvm::BasicBlock *BB);
        std::vector<Edge *> findEdgesStartingAtNode(Node *N);
        void removeNode(Node *N);
        void removeOrphanedEdges();
        void replaceNodesWithLoopNode(std::vector<llvm::BasicBlock *> blocks, LoopNode *LPN);
        double getEnergy(LLVMHandler *handler);
        void removeLoopEdges();
        std::vector<LoopNode *> getLoopNodes();
        bool containsLoopNodes();


};


#endif //BA_PROGRAMTREE_H
