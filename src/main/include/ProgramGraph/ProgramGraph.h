
#ifndef BA_PROGRAMGRAPH_H
#define BA_PROGRAMGRAPH_H


#include <vector>
#include <llvm/Analysis/RegionInfo.h>
#include "llvm/IR/BasicBlock.h"
#include "../LoopTree/LoopTree.h"
#include <cfloat>
#include "../AnalysisStrategy/AnalysisStrategy.h"


//Pre-declaration of the ProgramGraph Class
class ProgramGraph;

/**
 * Node - A Node in the Programtree. Represents a set of BasicBlocks, which may contain if-statements but is loop-free
 */
class Node {
    public:
        /**
         * Reference to the Tree this node is contained in
         */
        ProgramGraph *parent;

        /**
         * A vector savint references to the blocks in this node
         */
        std::vector<llvm::BasicBlock *> blocks;

        /**
         * The strategy the analysis should follow
         */
        AnalysisStrategy::Strategy strategy;

        /**
         * Constructor taking the surrounding ProgramGraph
         * @param parent
         */
        Node(ProgramGraph *parent, AnalysisStrategy::Strategy strategy);

        /**
         * Method for converting this node to string for debug output
         * Made virtual so further node specializations can be made
         * @return Returns this node a string
         */
        virtual std::string toString();

        /**
         * Method for calculating this Nodes energy consumption based on the given LLVMHandler
         * Made virtual, as calculation can differ for the specific node-type
         * @param handler A Reference to a LLVMHandler for calculating the energy usage
         * @return Returns the esitmated energy for this Node as double
         */
        virtual double getEnergy(LLVMHandler *handler);

protected:
    /**
     * Calculates the adjacent Nodes extending through vertices in the parent ProgramGraph from this Node outwards
     * @return Returns a vector of references to the adjacent nodes
     */
    std::vector<Node *> getAdjacentNodes();
};



/**
 * Edge - Class to represent the connection between two nodes. The connection is not directional by definition.
 */
class Edge {
public:
    /**
     * Reference to the starting node of the edge
     */
    Node * start;

    /**
     * Reference to the ending node of the edge
     */
    Node * end;

    /**
     * Constructor taking a start-node and an end-node
     * @param start Node, the edges starts on
     * @param end Node, the edge ends on
     */
    Edge(Node *start, Node *end);

    /**
     * Method for converting the edge to a string representation
     * @return
     */
    std::string toString() const;
};



/**
 * LoopNode - A specialization of the Node to represent loops extracted from the program. LoopNodes may contain further
 * ProgramTrees representing a recursive structure
 */
class LoopNode : public Node {
public:
    /**
     * Reference to a LoopTree object storing the information of the loop represented by this LoopNode
     */
    LoopTree *loopTree;

    /**
     * Vector storing the references to the subtrees encapsulated by this LoopNode
     */
    std::vector<ProgramGraph *> subtrees;

    /**
     * Constructor. Creates a LoopNode with the given LoopTree and the surrounding ProgramGraph
     * @param loopTree Reference to the LoopTree for the loop to represent
     * @param parent The parent ProgramGraph, this LoopNode is contained in
     */
    LoopNode(LoopTree *loopTree, ProgramGraph *parent, AnalysisStrategy::Strategy strategy);

    /**
     * Static Method for constructing a LoopNode recursivly trough the given LoopTree and the parent ProgramGraph
     * @param loopTree LoopTree to construct the LoopNode for
     * @param parent ProgramGraph the LoopNode should be contained in
     * @return Returns a reference to the constructed LoopNode
     */
    static LoopNode* construct(LoopTree *loopTree, ProgramGraph *parent, llvm::Function *function, AnalysisStrategy::Strategy strategy);

    /**
     * Method for determining if the current LoopNode is a "Leaf".
     * A leaf-LoopNode does not contain any further loops
     * @return Returns true if this LoopNode does not contain any further loops. False if otherwise
     */
    bool isLeafNode() const;

    /**
     * Method for calculating the energy of this LoopNode. Calculates the energy with respect to all contained subloops.
     * Multiplies the contained energy, by the iterations of this nodes loop.
     * Overrides the inherited getEnergy method
     * @param handler A LLVMHandler used for energy calculation
     * @return Returns the calculated energy as double
     */
    double getEnergy(LLVMHandler *handler) override;

    /**
     * Method for breaking cycles in the subtrees of this LoopNode.
     * Prevents infity-calculations while dealing with recursion.
     */
    void removeLoopEdgesFromSubtrees();

    /**
     * Method for representing the LoopNode as string.
     * Overrides the inherited method for better representing the contained programtrees
     * @return Returns a string representing the LoopNode
     */
    std::string toString() override;

    /**
     * LoopNodes Destructor
     */
    ~LoopNode();
};



/**
 * ProgramGraph -  Class to (partly) represent a program as a graph
 */
class ProgramGraph {
    public:
        /**
         * Vector containing references to the nodes contained in the graph
         */
        std::vector<Node *> nodes;

        /**
         * Vector containing references to the edges of the graph
         */
        std::vector<Edge *> edges;

        llvm::Function * parentFunction;

        /**
         * Static method for creating a ProgramGraph from a given set of BasicBlocks
         * @param blockset Vector with references to a set of basic blocks
         * @return Returns the constructed ProgramGraph
         */
        static ProgramGraph* construct(const std::vector<llvm::BasicBlock *>& blockset, llvm::Function *function, AnalysisStrategy::Strategy strategy);

        /**
         * ProgramGraph destructor
         */
        ~ProgramGraph();

        /**
         * Method for printing the string representations of the contained nodes with their calculated energy
         * @param handler A LLVMHandler used for energy calculation
         */
        void printNodes(LLVMHandler *handler);

        /**
         * Method for printing the string representations of the graphs edges
         */
        void printEdges();

        /**
         * Method for getting the Node contained in this ProgramGraph, which holds the given BasicBlock
         * @param basicBlock A reference to the BasicBlock to find
         * @return A reference to the Node if it was found. Returns a null pointer otherwise.
         */
        Node *findBlock(llvm::BasicBlock *basicBlock);

        /**
         * Calculates the edges going outwards from the given node.
         * @param sourceNode A reference to the Node the edges are extending from
         * @return Returns a vector of references to the edges
         */
        std::vector<Edge *> findEdgesStartingAtNode(Node *sourceNode);

        /**
         * Removes the given Node from this ProgramTre
         * @param nodeToRemove Reference to the Node to remove
         */
        void removeNode(Node *nodeToRemove);

        /**
         * Method for removing all edges from the graph, which start node or end node got removed
         */
        void removeOrphanedEdges();

        /**
         * Method for replacing the nodes in the ProgramGraph contained by a loopNode.
         * @param blocks Vector of BasicBlocks to replace by the given LoopNode
         * @param loopNode LoopNode used for replacing
         */
        void replaceNodesWithLoopNode(const std::vector<llvm::BasicBlock *>& blocks, LoopNode *loopNode);

        /**
         * Method for calculating the energy of this ProgramGraph. Uses the getEnergy() methods of the contained nodes
         * @param handler LLVMHandler used to the get the values for the basic blocks
         * @return Returns the used energy as double
         */
        double getEnergy(LLVMHandler *handler);

        /**
         * Calculates the LoopNodes contained in this ProgramGraph
         * @return Returns a Vector of references to the contained LoopNodes.
         */
        std::vector<LoopNode *> getLoopNodes();

        /**
         * Calculates if the ProgramGraph contains LoopNodes
         * @return Returns true if the ProgramGraph contains LoopNodes. False if otherwise
         */
        bool containsLoopNodes();
};


#endif //BA_PROGRAMGRAPH_H
