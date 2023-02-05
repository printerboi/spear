//
// Created by maximiliank on 03.02.23.
//

#include "ProgramTree.h"

ProgramTree* ProgramTree::construct(std::vector<llvm::BasicBlock *> blockset) {
    auto *PT = new ProgramTree();
    std::vector<llvm::BasicBlock *> bbs;


    for(auto BB : blockset){
        auto *NN = new Node();
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

void ProgramTree::printNodes() {

    for (auto N : this->nodes) {
        llvm::outs() << "\n----------------------------------------------------------------------\n";
        llvm::outs() << N->toString() << "\n";
        if(dynamic_cast<LoopNode*>(N) != nullptr){

            for (auto sPT : dynamic_cast<LoopNode*>(N)->subtrees) {
                llvm::outs() << "\n|\t\t\t\t\tBEGIN Subnodes\t\t\t\t\t|\n";
                sPT->printNodes();
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
    llvm::outs() << "Programtree got " << this->edges.size() << "\n";
    for (auto E : edges) {
        llvm::outs() << "\n";
        llvm::outs() << E->toString() << "\n";
    }
}

bool ProgramTree::replaceNodesWithLoopNode(std::vector<llvm::BasicBlock *> blocks, LoopNode *LPN) {
    std::vector<Node *> nodesToReplace;
    for (auto bb : blocks) {
        Node *toReplace = this->findBlock(bb);
        if(toReplace != nullptr){
            nodesToReplace.push_back(toReplace);
        }
    }


    this->nodes.push_back(LPN);

    if(!nodesToReplace.empty()){
        Node *entry = this->findBlock(LPN->loopTree->mainloop->getBlocksVector()[0]);
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
            for (auto bb : ntrpl->blocks) {
                bb->print(llvm::outs());
            }
        }
        this->removeOrphanedEdges();



        return true;
    }else{
        return false;
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

