//
// Created by maximiliank on 03.02.23.
//

#include "ProgramTree.h"

ProgramTree* ProgramTree::construct(std::vector<llvm::BasicBlock *> blockset) {
    auto *PT = new ProgramTree();
    std::vector<llvm::BasicBlock *> bbs;


    for(auto BB : blockset){
        auto *NN = new Node(PT);
        NN->blocks.push_back(BB);
        PT->nodes.push_back(NN);
    }

    for (auto BB : blockset) {
        for (auto succ : llvm::successors(BB)) {
            Node *start = PT->findBlock(BB);
            Node *end = PT->findBlock(succ);

            if(start != nullptr && end != nullptr){
                auto *E = new Edge(start, end);
                PT->edges.push_back(E);
            }
        }
    }

    return PT;
}

void ProgramTree::printNodes(LLVMHandler *handler) {

    for (auto N : this->nodes) {
        llvm::outs() << "\n----------------------------------------------------------------------\n";
        llvm::outs() << N->toString() <<  "\n";
        if(dynamic_cast<LoopNode*>(N) != nullptr){
            llvm::outs() << "(" << dynamic_cast<LoopNode*>(N)->loopTree->iterations << ") " << this->getEnergy(handler) << " µJ" << ")";
            for (auto sPT : dynamic_cast<LoopNode*>(N)->subtrees) {
                llvm::outs() << "\n|\t\t\t\t\tBEGIN Subnodes\t\t\t\t\t|\n";
                sPT->printNodes(handler);
                llvm::outs() << "\n|\t\t\t\t\tEND Subnodes\t\t\t\t\t|\n";
            }
        }
        llvm::outs() << "----------------------------------------------------------------------\n";
    }

}

Node *ProgramTree::findBlock(llvm::BasicBlock *BB) {
    for(auto Node : this->nodes){
        if(std::find(Node->blocks.begin(), Node->blocks.end(), BB) != Node->blocks.end()){
            return Node;
        }
    }

    return nullptr;
}

void ProgramTree::printEdges() {
    for (auto E : edges) {
        llvm::outs() << "\n";
        llvm::outs() << E->toString() << "\n";
    }

    for (auto N : this->nodes) {
        if(dynamic_cast<LoopNode*>(N) != nullptr){

            for (auto sPT : dynamic_cast<LoopNode*>(N)->subtrees) {
                llvm::outs() << "\n|\t\t\t\t\tBEGIN Subedges\t\t\t\t\t|\n";
                sPT->printEdges();
                llvm::outs() << "\n|\t\t\t\t\tEND Subedges\t\t\t\t\t|\n";
            }
        }
    }
}

void ProgramTree::replaceNodesWithLoopNode(std::vector<llvm::BasicBlock *> blocks, LoopNode *LPN) {
    std::vector<Node *> nodesToReplace;

    std::vector<std::string> allblocks;
    std::vector<std::string> toRemoveBlocks;

    for (auto bb : blocks) {
        Node *toReplace = this->findBlock(bb);
        if(toReplace != nullptr){
            nodesToReplace.push_back(toReplace);
            for(auto nb : toReplace->blocks){
                toRemoveBlocks.push_back(nb->getName().str());
            }
        }

        for(auto nb : toReplace->blocks){
            allblocks.push_back(nb->getName().str());
        }
    }



    this->nodes.push_back(LPN);

    if(!nodesToReplace.empty()){
        auto entrycandidate = LPN->loopTree->mainloop->getBlocksVector()[0];
        Node *entry = this->findBlock(entrycandidate);
        auto entryname = entry->toString();
        Node *exit = this->findBlock(LPN->loopTree->mainloop->getLoopLatch());
        auto exiname = exit->toString();


        for (auto edge : this->edges) {
            if(edge->end == entry){
                edge->end = LPN;
            }

            if(edge->start == exit){
                edge->start = LPN;
            }
        }

        for(auto ntrpl : nodesToReplace){
            this->removeNode(ntrpl);
        }
        this->removeOrphanedEdges();

    }
}

void ProgramTree::removeNode(Node *N) {
    std::vector<Node *> newNodes;

    for (auto node : this->nodes) {
        if(node != N){
            newNodes.push_back(node);
        }
    }

    this->nodes = newNodes;
}

void ProgramTree::removeOrphanedEdges() {
    std::vector<Edge *> cleanedEdges;

    for (auto edge : this->edges) {
        if(std::find(this->nodes.begin(), this->nodes.end(), edge->start) != this->nodes.end() && std::find(this->nodes.begin(), this->nodes.end(), edge->end) != this->nodes.end() ){
            if(edge->start != edge->end){
                cleanedEdges.push_back(edge);
            }
        }
    }

    this->edges = cleanedEdges;
}

double ProgramTree::getEnergy(LLVMHandler *handler) {
    double sum = 0.0;
    auto currnode = this->nodes[0];

    sum = currnode->getEnergy(handler);


    return sum;
}

std::vector<Edge *> ProgramTree::findEdgesStartingAtNode(Node *N) {
    std::vector<Edge *> selection;
    for(auto E : this->edges){
        if(E->start == N){
            selection.push_back(E);
        }
    }

    return selection;
}

void ProgramTree::removeLoopEdges() {
    for(auto lpn : this->getLoopNodes()){
        lpn->removeLoopEdgesFromSubtrees();
    }
}

bool ProgramTree::containsLoopNodes() {
    return !this->getLoopNodes().empty();
}

std::vector<LoopNode *> ProgramTree::getLoopNodes() {
    std::vector<LoopNode *> loopnodes;

    for(auto n : this->nodes){
        LoopNode *LPNCandicate = dynamic_cast<LoopNode *>(n);
        if(LPNCandicate != nullptr){
            loopnodes.push_back(LPNCandicate);
        }
    }

    return loopnodes;
}

