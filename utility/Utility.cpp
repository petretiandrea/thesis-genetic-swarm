//
// Created by Andrea Petreti on 04/01/2021.
//

#include "Utility.h"
#include <stdexcept>
#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>

vector<bool> utility::boolVectorFromString(const string& string) {
    int genomeSize = string.size();
    vector<bool> booleans;
    for(int i = 0; i < genomeSize; i++) {
        booleans.push_back(string[i] != '0');
    }
    return booleans;
}

vector<bool> utility::generateBinaryVector(int size, rnd::Random& rnd, float bias) {
    vector<bool> v(size);
    boost::range::generate(v, [&rnd, bias](){
        return rnd.next() <= bias;
    });
    return v;
}


int utility::boolVectorToInt(const vector<bool>& values) {
    return std::accumulate(values.begin(), values.end(), 0, [](int x, int y) { return (x << 1) + y; });
}

int utility::boolToInt(bool value, bool invert) { return invert ? (!value ? 1 : 0) : (value ? 1 : 0); }