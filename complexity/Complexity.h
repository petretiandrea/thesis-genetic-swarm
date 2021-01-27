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

}

#endif //SWARM_GEN_COMPLEXITY_H
