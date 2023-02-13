#include "ProgramTree.h"

//Create a Node by setting the parent property with the given ProgramTree
Node::Node(ProgramTree *parent) {
    this->parent = parent;
}

std::string Node::toString() {
    //Init the output-string
    std::string output;

    //Iterate over the contained blocks
    for (auto BB : this->blocks) {
        //Add the current basicblocks name to the output string
        output.append(BB->getName());
    }

    //Return the string
    return output;
}

//Calculate the energy of this Node. Is capable of dealing with if-conditions
double Node::getEnergy(LLVMHandler *handler) {
    //Init the result of the calculation
    double sum = 0.0;

    //Calculate the adjacent nodes of this node
    auto adjsNodes = this->getAdjacentNodes();

    //If there are adjacent nodes...
    if(!adjsNodes.empty()){
        //todo Distinguish between bestcase, wortcase and averagecase
        //Find the smallest energy-value-path of all the adjacent nodes
        //Init the minimal pathvalue
        double min = DBL_MAX;

        //Iterate over the adjacent nodes
        for(auto N : adjsNodes){
            //Calculate the sum of the node
            double locsum = N->getEnergy(handler);

            //Set the minimal energy value if the calculated energy is smaller than the current minimum
            if (locsum < min){
                min = locsum;
            }
        }

        //Add the minimal energy value to the energy-value of this node
        sum += min;
    }

    //Calculate the energy-cost of this node's basic blocks and add it to the sum
    for(auto &block : this->blocks){
        sum = sum + handler->getBasicBlockSum(*block);
    }

    //Return the calculated energy
    return sum;
}

//Calculate the adjacent Nodes
std::vector<Node *> Node::getAdjacentNodes() {
    //Init the vector
    std::vector<Node *> adjacent;

    //Get the edgdes starting at this node from the parent ProgramTree
    for(auto E : this->parent->findEdgesStartingAtNode(this)){
        //Add the end of the edge to the adjacent vector
        adjacent.push_back(E->end);
    }

    //Return the adjacent nodes vector
    return adjacent;
}