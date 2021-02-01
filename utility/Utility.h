//
// Created by Andrea Petreti on 04/01/2021.
//

#ifndef SWARM_GEN_UTILITY_H
#define SWARM_GEN_UTILITY_H

#include <vector>
#include "Random.h"
#include "Matrix.h"

using namespace std;

namespace utility {

    vector<bool> generateBinaryVector(int size, rnd::Random& rnd, float bias);
    vector<bool> boolVectorFromString(const string& string);
    int boolToInt(bool value, bool invert = false);

    int boolVectorToInt(const vector<bool>& values);

    template<typename T> static Matrix<T> vectorToMatrix(const vector<T>& vector, int row, int column) {
        if(row * column > vector.size()) throw std::invalid_argument("vector size must be enough for row x column matrix");
        if(row * column < vector.size()) std::cout << "Warn: some element of vector where dropped" << std::endl;

        Matrix<T> matrix(row, column);
        for(int i = 0; i < row; i++) {
            for (int j = 0; j < column; j++) {
                matrix.put(i, j, vector[i * column + j]);
            }
        }
        return matrix;
    }

    template<typename T> static Matrix<T> vectorToMatrix(const vector<T>& vector, int column) {
        return utility::vectorToMatrix(vector, vector.size() / column, column);
    }

    template<typename T> static std::vector<T> extractFromCollection(rnd::Random& rnd, std::vector<T> collection, int numberOfElements, bool allowRepetition = false) {
        std::vector<T> result;
        std::vector<T> toConsider(collection);

        for(int i = 0; i < numberOfElements; i++) {
            if (allowRepetition) {
                result.push_back(toConsider[rnd.nextInt(0, toConsider.size() - 1)]);
            } else {
                auto indexElem = rnd.nextInt(0, toConsider.size() - 1);
                result.push_back(toConsider[indexElem]);
                toConsider.erase(toConsider.begin() + indexElem);
            }
        }
        return result;
    }
}

template<typename T> std::ostream& operator<<(std::ostream& os, const vector<T>& vector) {
    os << "[";
    for(int i = 0; i < vector.size() - 1; i++) {
        os << vector[i] << ", ";
    }
    os << vector[vector.size() - 1] << "]";
    return os;
}

#endif //SWARM_GEN_UTILITY_H
