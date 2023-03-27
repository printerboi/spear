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
    for (auto basicBlock : this->blocks) {
        //Add the current basicblocks name to the output string
        output.append(basicBlock->getName().str());
    }

    //Return the string
    return output;
}

//Calculate the energy of this Node. Is capable of dealing with if-conditions
double Node::getEnergy(LLVMHandler *handler) {
    //Init the result of the calculation
    double sum = 0.0;

    //Calculate the adjacent nodes of this node
    auto adjacentNodes = this->getAdjacentNodes();

    //If there are adjacent nodes...
    if(!adjacentNodes.empty()){
        //Find the smallest energy-value-path of all the adjacent nodes
        //Init the minimal pathvalue
        auto compare = 0.00;

        switch (this->strategy) {
            case AnalysisStrategy::WORSTCASE :
                 compare = DBL_MIN;

                //Iterate over the adjacent nodes
                for(auto node : adjacentNodes){
                    //Calculate the sum of the node
                    double locsum = node->getEnergy(handler);

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
                for(auto node : adjacentNodes){
                    //Calculate the sum of the node
                    double locsum = node->getEnergy(handler);

                    //Set the minimal energy value if the calculated energy is smaller than the current minimum
                    if (locsum < compare){
                        compare = locsum;
                    }
                }

                sum += compare;
                break;
            case AnalysisStrategy::AVERAGECASE :
                double locsum = 0.00;

                if(adjacentNodes.size() > 1){
                    double leftSum = adjacentNodes[0]->getEnergy(handler);
                    double rightSum = adjacentNodes[1]->getEnergy(handler);

                    if(handler->inefficient <= handler->efficient){
                        locsum += std::max(leftSum, rightSum);
                        handler->inefficient++;
                    }else{
                        locsum += std::min(leftSum, rightSum);
                        handler->efficient++;
                    }
                }else{
                    locsum = adjacentNodes[0]->getEnergy(handler);
                }

/*                srand(time(nullptr));
                int randomIndex = rand() % adjacentNodes.size();
                double locsum = adjacentNodes[randomIndex]->getEnergy(handler);
                compare = locsum;
                sum += compare;*/
                sum += locsum;

                break;
        }

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

    //Get the edgdes starting at this node from the parent ProgramGraph
    for(auto edge : this->parent->findEdgesStartingAtNode(this)){
        //Add the end of the edge to the adjacent vector
        adjacent.push_back(edge->end);
    }

    //Return the adjacent nodes vector
    return adjacent;
}