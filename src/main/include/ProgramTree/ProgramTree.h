//
// Created by maximiliank on 03.02.23.
//

#ifndef BA_PROGRAMTREE_H
#define BA_PROGRAMTREE_H


#include <vector>
#include <llvm/Analysis/RegionInfo.h>
#include "llvm/IR/BasicBlock.h"
#include "../LoopTree/LoopTree.h"

class ProgramTree;

class Node {
public:
    std::vector<llvm::BasicBlock *> predecessors;
    std::vector<llvm::BasicBlock *> successors;
    std::vector<llvm::BasicBlock *> blocks;


    virtual std::string toString();
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
    LoopNode(LoopTree *LT);
    LoopTree *loopTree;
    std::vector<ProgramTree *> subtrees;
    static LoopNode* construct(LoopTree *lptr);
    bool isLeafNode();


    std::string toString();
};

class ProgramTree {
    public:
        std::vector<Node *> nodes;
        std::vector<Edge *> edges;

        static ProgramTree* construct(std::vector<llvm::BasicBlock *> blockset);

        void printNodes();
        void printEdges();
        Node *findBlock(llvm::BasicBlock *BB);
        void removeNode(Node *N);
        void removeOrphanedEdges();
        bool replaceNodesWithLoopNode(std::vector<llvm::BasicBlock *> blocks, LoopNode *LPN);
};


#endif //BA_PROGRAMTREE_H
