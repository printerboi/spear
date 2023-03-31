#include "ProgramGraph.h"

//Static method for ProgramGraph-Graph construction
ProgramGraph* ProgramGraph::construct(const std::vector<llvm::BasicBlock *>& blockset, llvm::Function *function, AnalysisStrategy::Strategy strategy) {
    //Create a dummy-Object
    auto *programGraph = new ProgramGraph();
    //Create an empty list for the BasicBlocks
    std::vector<llvm::BasicBlock *> basicBlockSet;

    //Iterate over the given list of BasicBlock-References
    for(auto basicBlock : blockset){
        //For each block create a new Node
        auto *node = new Node(programGraph, strategy);
        //Add the block to the Node
        node->block = basicBlock;
        //Add the node to the graph
        programGraph->nodes.push_back(node);
    }

    //Iterate over the blocks to create the edges of the graph
    for (auto basicBlock : blockset) {
        //Determine the successors to the current block in the cfg
        for (auto successor : llvm::successors(basicBlock)) {
            //Get the nodes defining the edge
            Node *start = programGraph->findBlock(basicBlock);
            Node *end = programGraph->findBlock(successor);

            //If both of the defining nodes were found
            if(start != nullptr && end != nullptr){
                //Create the edge
                auto *edge = new Edge(start, end);
                //Add the edge to the edge-list of the graph
                programGraph->edges.push_back(edge);
            }
        }
    }

    //Return the graph
    return programGraph;
}

ProgramGraph::~ProgramGraph() {
    for (auto node : this->nodes) {
        delete node;
    }

    for (auto edge : this->edges) {
        delete edge;
    }
}

//Print the Graph in preorder
void ProgramGraph::printNodes(LLVMHandler *handler) {
    //Iterate over the nodes in the graph
    for (auto node : this->nodes) {
        llvm::outs() << "\n----------------------------------------------------------------------\n";
        llvm::outs() << node->toString() << "\n";
        //If the current Node is a LoopNode...
        if(dynamic_cast<LoopNode*>(node) != nullptr){
            //Print a special representation if we are dealing with a LoopNode
            llvm::outs() << "(" << dynamic_cast<LoopNode*>(node)->loopTree->iterations << ") " << this->getEnergy(handler) << " µJ" << ")";
            for (auto subProgramGraph : dynamic_cast<LoopNode*>(node)->subgraphs) {
                llvm::outs() << "\n|\t\t\t\t\tBEGIN Subnodes\t\t\t\t\t|\n";
                subProgramGraph->printNodes(handler);
                llvm::outs() << "\n|\t\t\t\t\tEND Subnodes\t\t\t\t\t|\n";
            }
        }
        llvm::outs() << "----------------------------------------------------------------------\n";
    }

}

//Search for the given block in the graph
Node *ProgramGraph::findBlock(llvm::BasicBlock *basicBlock) {
    //Iterate over the nodes
    for(auto Node : this->nodes){

        //Use the standard find method to search the vector
        if(Node->block == basicBlock){
            //If found return the node
            return Node;
        }
    }

    //If nothing was found, return a null pointer
    return nullptr;
}

//Print te edges of the graph
void ProgramGraph::printEdges() {
    //Iterate over the edges and use the toString() method of the edges
    for (auto edge : edges) {
        llvm::outs() << "\n";
        llvm::outs() << edge->toString() << "\n";
    }

    //Iterate over the nodes of the graph
    for (auto node : this->nodes) {
        //If we are dealing with a LoopNode
        if(dynamic_cast<LoopNode*>(node) != nullptr){
            //Print the edges contained in the LoopNode
            for (auto subProgramGraph : dynamic_cast<LoopNode*>(node)->subgraphs) {
                llvm::outs() << "\n|\t\t\t\t\tBEGIN Subedges\t\t\t\t\t|\n";
                subProgramGraph->printEdges();
                llvm::outs() << "\n|\t\t\t\t\tEND Subedges\t\t\t\t\t|\n";
            }
        }
    }
}

//Replaces the given blocks with the given loopnode
void ProgramGraph::replaceNodesWithLoopNode(const std::vector<llvm::BasicBlock *>& blocks, LoopNode *loopNode) {
    //Init the list of nodes, that need replacement
    std::vector<Node *> nodesToReplace;

    //Iterate over the given blocks
    for (auto basicBlock : blocks) {
        //Find the corresponding nodes to the blocks
        Node *toReplace = this->findBlock(basicBlock);

        //If a Node for the block was found...
        if(toReplace != nullptr){
            //Add the node to the list
            nodesToReplace.push_back(toReplace);
        }

    }

    //Add the given LoopNode to the list of nodes for this graph
    this->nodes.push_back(loopNode);

    //If we need to replace some nodes...
    if(!nodesToReplace.empty()){
        //Get the entry-block of the LoopNode
        auto entrycandidate = loopNode->loopTree->mainloop->getBlocksVector()[0];
        //Find the node corresponding to this block
        Node *entry = this->findBlock(entrycandidate);
        auto entryname = entry->toString();

        //Get the exit-node of the LoopNode
        Node *exit = this->findBlock(loopNode->loopTree->mainloop->getLoopLatch());
        auto exiname = exit->toString();

        //Iterate over the edges of this graph
        for (auto edge : this->edges) {
            //If we find an edge, which end-node is the entry-node of the loop
            if(edge->end == entry){
                //Change the edges endpoint to the LoopNode
                edge->end = loopNode;
            }

            //If we find an edge, which start-node is the exit-node of the loop
            if(edge->start == exit){
                //Change the edges startpoint to the LoopNode
                edge->start = loopNode;
            }
        }

        //Remove the nodes encapsulated by the loop
        for(auto node : nodesToReplace){
            this->removeNode(node);
        }

        //Take care of all edges, that may be orphaned after the editing of the graph
        this->removeOrphanedEdges();
    }
}

//Remove a given Node from the graph
void ProgramGraph::removeNode(Node *nodeToRemove) {
    //Init the list of Nodes we want to keep
    std::vector<Node *> newNodes;

    //Iterate over the nodes of the graph
    for (auto node : this->nodes) {
        //Add the nodes to the keep-list if it is unequal to the given Node
        if(node != nodeToRemove){
            newNodes.push_back(node);
        }
    }

    //Set the nodes of the graph to the keep-list
    this->nodes = newNodes;
}

//Removes all edges from the graph, that refere to nodes no longer present in the graph
void ProgramGraph::removeOrphanedEdges() {
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
double ProgramGraph::getEnergy(LLVMHandler *handler) {
    //Init the calculation result
    double sum = 0.0;
    //Get the first node of the graph
    auto currentNode = this->nodes[0];

    //Start the energy-calculation from the start node
    //Uses the recursvice calculation of the nodes by itself
    sum = currentNode->getNodeEnergy(handler);

    //Return the result
    return sum;
}

//Find all the edges starting at the given Node
std::vector<Edge *> ProgramGraph::findEdgesStartingAtNode(Node *sourceNode) {
    //Init the list
    std::vector<Edge *> selection;

    //Iterate over the edges
    for(auto edge : this->edges){
        //If the start of the edge is the given Node
        if(edge->start == sourceNode){
            //Add the edge to the list
            selection.push_back(edge);
        }
    }

    //Return the list of edges
    return selection;
}

//Test if thie graph contains a LoopNode
bool ProgramGraph::containsLoopNodes() {
    return !this->getLoopNodes().empty();
}

//Get all the LoopNodes contained in the Graph
std::vector<LoopNode *> ProgramGraph::getLoopNodes() {
    //Init the list
    std::vector<LoopNode *> loopnodes;

    //Iterate over the nodes
    for(auto node : this->nodes){
        //Test if the current node is a LoopNode
        auto *loopNodeCandidate = dynamic_cast<LoopNode *>(node);
        if(loopNodeCandidate != nullptr){
            //If we're dealing with a LoopNode, add it to the list
            loopnodes.push_back(loopNodeCandidate);
        }
    }

    //Return the list of LoopNodes
    return loopnodes;
}