//
// Created by Andrea Petreti on 04/01/2021.
//

#ifndef SWARM_GEN_BOOLEANNETWORK_H
#define SWARM_GEN_BOOLEANNETWORK_H

#include <vector>
#include <utility/Random.h>
#include <utility/Matrix.h>

using namespace std;

class BooleanNetwork {

public:
    static std::function<bool()> booleanFunctionsInitializer(rnd::Random* random, float bias) {
        return [random, bias]() -> bool { return random->next() <= bias; };
    }

public:
    BooleanNetwork(int n, int k, float bias, int inputNode, int outputNode, int seed, bool selfLoops = false);
    BooleanNetwork(Matrix<bool>& booleanFunctions, Matrix<int>& connections, int inputNode, int outputNode);

    /** Update the input nodes of network */
    void forceInputValues(std::vector<bool> inputs);

    /** Update the input node to specified value */
    void forceInputValue(int index, bool value);

    /** It compute one step of network. */
    void update();

    vector<bool> getInputValues();

    /** Get the values of output nodes. */
    vector<bool> getOutputValues();

    /** Set the boolean functions to specified value */
    void changeBooleanFunction(const vector<bool>& booleanFunctions);
    void changeBooleanFunction(const Matrix<bool>& booleanFunctions);


    /** Restore the states of nodes to initial state (0) */
    void resetStates();


public:
    /**
     * Get the boolean functions of network as a matrix N x 2^K
     */
    inline const Matrix<bool>& getBooleanFunctions() { return booleanFunctions; }

    /**
     * Get the connections matrix of network as a matrix N x K
     */
    inline const Matrix<int>& getConnections() { return connectionMatrix; }

    /**
     * Get the nodes marked as input of network
     */
    inline const vector<int>& getInputNodes() { return inputNodes; }

    /**
     * Get the nodes marked as output of network
     */
    inline const vector<int>& getOutputNodes() { return outputNodes; }

    /**
     * Total number of nodes of network
     */
    inline int getNumberOfNodes() const { return connectionMatrix.getRows(); }

    /**
     * Get number of input for node (K)
     */
    inline int getInputForNode() const { return connectionMatrix.getColumns(); }

public:
    static Matrix<int> createRandomConnectionMatrix(rnd::Random& rnd, int totalNodes, int inputsForNode, bool selfLoop);

private:
    static vector<int> extractNodeInputIndexes(rnd::Random& rnd, int nodeToLink, vector<int> nodeIndexes, int inputsForNode, bool selfLoop);


public:
    vector<bool> states;

private:
    Matrix<bool> booleanFunctions;
    Matrix<int> connectionMatrix;
    vector<int> inputNodes;
    vector<int> outputNodes;


    /** Initialize the network */
    void init(Matrix<bool>& booleanFunctions, Matrix<int>& connections, int inputNode, int outputNode);
    bool calculateNodeUpdate(int nodeIndex, const vector<bool>& oldStates);
};

#endif //SWARM_GEN_BOOLEANNETWORK_H
