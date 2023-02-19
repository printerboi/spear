#include "ProgramGraph.h"

//Create a Node by setting the parent property with the given ProgramGraph
Node::Node(ProgramGraph *parent, AnalysisStrategy::Strategy strategy) {
    this->parent = parent;
    this->strategy = strategy;
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
        //Find the smallest energy-value-path of all the adjacent nodes
        //Init the minimal pathvalue
        auto compare = 0.00;

        switch (this->strategy) {
            case AnalysisStrategy::WORSTCASE :
                 compare = DBL_MIN;

                //Iterate over the adjacent nodes
                for(auto N : adjsNodes){
                    //Calculate the sum of the node
                    double locsum = N->getEnergy(handler);

                    //Set the minimal energy value if the calculated energy is smaller than the current minimum
                    if (locsum > compare){
                        compare = locsum;
                    }
                }

                sum += compare;
                break;
            case AnalysisStrategy::BESTCASE :
                compare = DBL_MAX;

                //Iterate over the adjacent nodes
                for(auto N : adjsNodes){
                    //Calculate the sum of the node
                    double locsum = N->getEnergy(handler);

                    //Set the minimal energy value if the calculated energy is smaller than the current minimum
                    if (locsum < compare){
                        compare = locsum;
                    }
                }

                sum += compare;
                break;
            case AnalysisStrategy::AVERAGECASE :
                compare = 0.00;

                srand(time(nullptr));
                int randomIndex = rand() % adjsNodes.size();
                double locsum = adjsNodes[randomIndex]->getEnergy(handler);
                compare = locsum;
                sum += compare;

                break;
        }

    }

    //Calculate the energy-cost of this node's basic blocks and add it to the sum
    for(auto &block : this->blocks){
        sum = sum + handler->getBasicBlockSum(*block, this->parent->parentFunction);
    }

    //Return the calculated energy
    return sum;
}

//Calculate the adjacent Nodes
std::vector<Node *> Node::getAdjacentNodes() {
    //Init the vector
    std::vector<Node *> adjacent;

    //Get the edgdes starting at this node from the parent ProgramGraph
    for(auto E : this->parent->findEdgesStartingAtNode(this)){
        //Add the end of the edge to the adjacent vector
        adjacent.push_back(E->end);
    }

    //Return the adjacent nodes vector
    return adjacent;
}