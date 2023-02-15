#include "ProgramTree.h"

//Static method for ProgramTree-Graph construction
ProgramTree* ProgramTree::construct(std::vector<llvm::BasicBlock *> blockset, llvm::Function *func) {
    //Create a dummy-Object
    auto *PT = new ProgramTree();
    //Create an empty list for the BasicBlocks
    std::vector<llvm::BasicBlock *> bbs;
    PT->parentFunction = func;

    //Iterate over the given list of BasicBlock-References
    for(auto BB : blockset){
        //For each block create a new Node
        auto *NN = new Node(PT);
        //Add the block to the Node
        NN->blocks.push_back(BB);
        //Add the node to the graph
        PT->nodes.push_back(NN);
    }

    //Iterate over the blocks to create the edges of the graph
    for (auto BB : blockset) {
        //Determine the successors to the current block in the cfg
        for (auto succ : llvm::successors(BB)) {
            //Get the nodes defining the edge
            Node *start = PT->findBlock(BB);
            Node *end = PT->findBlock(succ);

            //If both of the defining nodes were found
            if(start != nullptr && end != nullptr){
                //Create the edge
                auto *E = new Edge(start, end);
                //Add the edge to the edge-list of the graph
                PT->edges.push_back(E);
            }
        }
    }

    //Return the graph
    return PT;
}

ProgramTree::~ProgramTree() {
    for (auto N : this->nodes) {
        delete N;
    }

    for (auto E : this->edges) {
        delete E;
    }
}

//Print the Graph in preorder
void ProgramTree::printNodes(LLVMHandler *handler) {
    //Iterate over the nodes in the graph
    for (auto N : this->nodes) {
        llvm::outs() << "\n----------------------------------------------------------------------\n";
        llvm::outs() << N->toString() <<  "\n";
        //If the current Node is a LoopNode...
        if(dynamic_cast<LoopNode*>(N) != nullptr){
            //Print a special representation if we are dealing with a LoopNode
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

//Search for the given block in the graph
Node *ProgramTree::findBlock(llvm::BasicBlock *BB) {
    //Iterate over the nodes
    for(auto Node : this->nodes){

        //Use the standard find method to search the vector
        if(std::find(Node->blocks.begin(), Node->blocks.end(), BB) != Node->blocks.end()){
            //If found return the node
            return Node;
        }
    }

    //If nothing was found, return a null pointer
    return nullptr;
}

//Print te edges of the graph
void ProgramTree::printEdges() {
    //Iterate over the edges and use the toString() method of the edges
    for (auto E : edges) {
        llvm::outs() << "\n";
        llvm::outs() << E->toString() << "\n";
    }

    //Iterate over the nodes of the graph
    for (auto N : this->nodes) {
        //If we are dealing with a LoopNode
        if(dynamic_cast<LoopNode*>(N) != nullptr){
            //Print the edges contained in the LoopNode
            for (auto sPT : dynamic_cast<LoopNode*>(N)->subtrees) {
                llvm::outs() << "\n|\t\t\t\t\tBEGIN Subedges\t\t\t\t\t|\n";
                sPT->printEdges();
                llvm::outs() << "\n|\t\t\t\t\tEND Subedges\t\t\t\t\t|\n";
            }
        }
    }
}

//Replaces the given blocks with the given loopnode
void ProgramTree::replaceNodesWithLoopNode(std::vector<llvm::BasicBlock *> blocks, LoopNode *LPN) {
    //Init the list of nodes, that need replacement
    std::vector<Node *> nodesToReplace;
    std::vector<std::string> allblocks;
    std::vector<std::string> toRemoveBlocks;

    //Iterate over the given blocks
    for (auto bb : blocks) {
        //Find the corresponding nodes to the blocks
        Node *toReplace = this->findBlock(bb);

        //If a Node for the block was found...
        if(toReplace != nullptr){
            //Add the node to the list
            nodesToReplace.push_back(toReplace);
            for(auto nb : toReplace->blocks){
                toRemoveBlocks.push_back(nb->getName().str());
            }
        }

        for(auto nb : toReplace->blocks){
            allblocks.push_back(nb->getName().str());
        }
    }

    //Add the given LoopNode to the list of nodes for this graph
    this->nodes.push_back(LPN);

    //If we need to replace some nodes...
    if(!nodesToReplace.empty()){
        //Get the entry-block of the LoopNode
        auto entrycandidate = LPN->loopTree->mainloop->getBlocksVector()[0];
        //Find the node corresponding to this block
        Node *entry = this->findBlock(entrycandidate);
        auto entryname = entry->toString();

        //Get the exit-node of the LoopNode
        Node *exit = this->findBlock(LPN->loopTree->mainloop->getLoopLatch());
        auto exiname = exit->toString();

        //Iterate over the edges of this graph
        for (auto edge : this->edges) {
            //If we find an edge, which end-node is the entry-node of the loop
            if(edge->end == entry){
                //Change the edges endpoint to the LoopNode
                edge->end = LPN;
            }

            //If we find an edge, which start-node is the exit-node of the loop
            if(edge->start == exit){
                //Change the edges startpoint to the LoopNode
                edge->start = LPN;
            }
        }

        //Remove the nodes encapsulated by the loop
        for(auto ntrpl : nodesToReplace){
            this->removeNode(ntrpl);
        }

        //Take care of all edges, that may be orphaned after the editing of the graph
        this->removeOrphanedEdges();
    }
}

//Remove a given Node from the graph
void ProgramTree::removeNode(Node *N) {
    //Init the list of Nodes we want to keep
    std::vector<Node *> newNodes;

    //Iterate over the nodes of the graph
    for (auto node : this->nodes) {
        //Add the nodes to the keep-list if it is unequal to the given Node
        if(node != N){
            newNodes.push_back(node);
        }
    }

    //Set the nodes of the graph to the keep-list
    this->nodes = newNodes;
}

//Removes all edges from the graph, that refere to nodes no longer present in the graph
void ProgramTree::removeOrphanedEdges() {
    //Init the list of edges, we want to keep
    std::vector<Edge *> cleanedEdges;

    //Iterate over the edges
    for (auto edge : this->edges) {
        //Test if the end node and the start node both can be found in the graph
        if(std::find(this->nodes.begin(), this->nodes.end(), edge->start) != this->nodes.end() &&
        std::find(this->nodes.begin(), this->nodes.end(), edge->end) != this->nodes.end() ){
            //If we have and edge without orphaned nodes. Test for self references
            if(edge->start != edge->end){
                //If we aren't having a self reference, add the node to the keep-list
                cleanedEdges.push_back(edge);
            }
        }
    }

    //Set the edges of the graph to the keeping edges
    this->edges = cleanedEdges;
}

//Calculate the energy of the graph
double ProgramTree::getEnergy(LLVMHandler *handler) {
    //Init the calculation result
    double sum = 0.0;
    //Get the first node of the graph
    auto currnode = this->nodes[0];

    //Start the energy-calculation from the start node
    //Uses the recursvice calculation of the nodes by itself
    sum = currnode->getEnergy(handler);

    //Return the result
    return sum;
}

//Find all the edges starting at the given Node
std::vector<Edge *> ProgramTree::findEdgesStartingAtNode(Node *N) {
    //Init the list
    std::vector<Edge *> selection;

    //Iterate over the edges
    for(auto E : this->edges){
        //If the start of the edge is the given Node
        if(E->start == N){
            //Add the edge to the list
            selection.push_back(E);
        }
    }

    //Return the list of edges
    return selection;
}

//Test if thie graph contains a LoopNode
bool ProgramTree::containsLoopNodes() {
    return !this->getLoopNodes().empty();
}

//Get all the LoopNodes contained in the Graph
std::vector<LoopNode *> ProgramTree::getLoopNodes() {
    //Init the list
    std::vector<LoopNode *> loopnodes;

    //Iterate over the nodes
    for(auto n : this->nodes){
        //Test if the current node is a LoopNode
        auto *LPNCandicate = dynamic_cast<LoopNode *>(n);
        if(LPNCandicate != nullptr){
            //If we're dealing with a LoopNode, add it to the list
            loopnodes.push_back(LPNCandicate);
        }
    }

    //Return the list of LoopNodes
    return loopnodes;
}