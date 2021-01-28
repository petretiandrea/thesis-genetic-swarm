//
// Created by Andrea Petreti on 04/01/2021.
//

#include "BooleanNetwork.h"
#include <boost/range.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/algorithm.hpp>
#include <utility/Utility.h>

namespace range = boost::range;

BooleanNetwork::BooleanNetwork(int n, int k, float bias, int inputNode, int outputNode, int seed, bool selfLoops) {
    auto rnd = rnd::Random(seed);
    auto functionLength = (int) pow(2, k);
    auto booleanVector = utility::generateBinaryVector(n * functionLength, rnd, bias);
    auto booleanFunctions = utility::vectorToMatrix(booleanVector, n, functionLength);

    auto connections = createRandomConnectionMatrix(rnd, n, k, selfLoops);

    this->init(booleanFunctions,
               connections,
               inputNode,
               outputNode);
}

BooleanNetwork::BooleanNetwork(Matrix<bool>& booleanFunctions, Matrix<int>& connections, int inputNode, int outputNode) {
    this->init(booleanFunctions, connections, inputNode, outputNode);
}

void BooleanNetwork::init(Matrix<bool>& booleanFunctions, Matrix<int>& connections, int inputNode, int outputNode) {
    this->connectionMatrix = connections;
    this->booleanFunctions = booleanFunctions;

    states = vector<bool>(getNumberOfNodes());
    inputNodes = vector<int>(inputNode);
    outputNodes = vector<int>(outputNode);
    range::fill_n(states, states.size(), false);

    /* first N nodes are used for input */
    range::transform(
            boost::irange(0, inputNode),
            inputNodes.begin(),
            [](int i) { return i; });

    range::transform(boost::irange(0, outputNode),
                     outputNodes.begin(),
                     [this](int index) { return this->getNumberOfNodes() - index - 1; });

    /*std::cout << booleanFunctions << std::endl;
    std::cout << connectionMatrix << std::endl;
    cout << states << endl;
    cout << "Input nodes " << inputNodes << endl;
    cout << "Output nodes " << outputNodes << endl;*/
}

void BooleanNetwork::forceInputValues(std::vector<bool> inputs) {
    if(inputs.size() > getInputNodes().size()) { cerr << "Forcing too inputs than declared " << endl; return; }
    for(int i = 0; i < inputs.size(); i++) {
        this->states[inputNodes[i]] = inputs[i];
    }
}

void BooleanNetwork::forceInputValue(int index, bool value) {
    if(index >= getInputNodes().size()) { cerr << "Index of input is out of range " << endl; return; }
    this->states[inputNodes[index]] = value;
}

void BooleanNetwork::update() {
    auto oldStates = vector<bool>(states); // this allow to update synchronously
    for(int i = 0; i < getNumberOfNodes(); i++) {
        states[i] = calculateNodeUpdate(i, oldStates);
    }
}

bool BooleanNetwork::calculateNodeUpdate(int nodeIndex, const vector<bool>& oldStates) {
    bool inputValues[connectionMatrix.getColumns()];
    for(int i = 0; i < connectionMatrix.getColumns(); i++) {
        inputValues[i] = oldStates[connectionMatrix(nodeIndex, i)];
    }

    int sum = 0;
    for(int i = 0; i < connectionMatrix.getColumns(); i++) {
        sum += utility::boolToInt(inputValues[i]) * ((int) pow(2, i));
    }

    return booleanFunctions.get(nodeIndex, sum);
}

std::vector<bool> BooleanNetwork::getOutputValues() {
    std::vector<bool> output(getOutputNodes().size());
    for(int i = 0; i < output.size(); i++) {
        output[i] = states[outputNodes[i]];
    }
    return output;
}

vector<bool> BooleanNetwork::getInputValues() {
    std::vector<bool> input(getInputNodes().size());
    for(int i = 0; i < input.size(); i++) {
        input[i] = states[inputNodes[i]];
    }
    return input;
}


Matrix<int> BooleanNetwork::createRandomConnectionMatrix(rnd::Random& rnd, int totalNodes, int inputsForNode, bool selfLoop) {
    Matrix<int> connectionMatrix(totalNodes, inputsForNode);
    vector<int> nodes = boost::copy_range<vector<int>>(boost::irange(0, totalNodes));

    for(int i = 0; i < connectionMatrix.getRows(); i++) {
        auto connections = extractNodeInputIndexes(rnd, i, nodes, inputsForNode, selfLoop);
        for(int j = 0; j < connections.size(); j++) connectionMatrix.put(i, j, connections[j]);
    }
    return connectionMatrix;
}

std::vector<int> BooleanNetwork::extractNodeInputIndexes(rnd::Random& rnd, int nodeToLink, std::vector<int> nodeIndexes, int inputsForNode, bool selfLoop) {
    // remove from considered node the inputNode if the self loop are not admitted
    if(!selfLoop) nodeIndexes.erase(std::remove(nodeIndexes.begin(), nodeIndexes.end(), nodeToLink), nodeIndexes.end());
    return utility::extractFromCollection(rnd, nodeIndexes, inputsForNode);
}


void BooleanNetwork::resetStates() {
    // reset the state to initial state (false)
    range::fill_n(states, states.size(), false);
}

void BooleanNetwork::changeBooleanFunction(const Matrix<bool> &newBooleanFunctions) {
    this->booleanFunctions = Matrix<bool>(newBooleanFunctions);
//    cout << "Boolean function changed " << endl;
//    cout << this->booleanFunctions << endl;
}

void BooleanNetwork::changeBooleanFunction(const vector<bool>& newBooleanFunctions) {
    this->changeBooleanFunction(utility::vectorToMatrix(newBooleanFunctions, this->booleanFunctions.getRows(), this->booleanFunctions.getColumns()));
}