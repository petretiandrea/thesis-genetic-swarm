//
// Created by Andrea Petreti on 27/01/2021.
//

#ifndef SWARM_GEN_COMPLEXITY_H
#define SWARM_GEN_COMPLEXITY_H

#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <set>
#include <iostream>
#include <utility/Utility.h>

using namespace std;

namespace complexity {

    inline double shannonEntropyFromString(const string& sequence, const std::set<char>& charset) {
        double entropy = 0;
        vector<double> probability;

        for(auto symbol : charset) {
            auto count = (double) std::count(sequence.begin(), sequence.end(), symbol);
            probability.push_back(count / sequence.length());
        }

        for(auto p : probability) {
            entropy += p * log2(p);
        }

        return -entropy;
    }

    inline double shannonEntropyFromString(const string& sequence) {
        std::set<char> charset(sequence.begin(), sequence.end());
        return shannonEntropyFromString(sequence, charset);
    }

    inline double shannon2(const vector<vector<bool>>& sequence, const std::set<vector<bool>>& symbols) {
        double entropy = 0;
        vector<double> probability;

        // esistemate prob function from frequency
        for(auto& symbol : symbols) {
            auto count = (double) std::count(sequence.begin(), sequence.end(), symbol);
            probability.push_back(count / sequence.size());
        }

        for(auto p : probability) {
            entropy += p * log2(p);
        }

        return -entropy;
    }

    inline double shannon2(const vector<vector<bool>>& sequence) {
        std::set<vector<bool>> charset(sequence.begin(), sequence.end());
        return shannon2(sequence, charset);
    }

}

#endif //SWARM_GEN_COMPLEXITY_H
