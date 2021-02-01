//
// Created by Andrea Petreti on 20/01/2021.
//

#ifndef SWARM_GEN_BOOLEANNETWORKLOADER_H
#define SWARM_GEN_BOOLEANNETWORKLOADER_H

#include <string>
#include <utility/Utility.h>
#include <core/SolutionLoader.h>

class BooleanNetworkLoader {

public:
    static vector<bool> loadBestBooleanFunctions(const string& filename) {
        std::cout << "parsing network from statistics file..." << endl;

        auto* functions = SolutionLoader::loadSolutionsLog(filename);

        // TODO: remove this and implement ordering
        assert(functions->size() == 1);

        auto best = functions->back();

        cout << "found network with fitness " << best.fitness << endl;
        return best.solution;
    }
};

#endif //SWARM_GEN_BOOLEANNETWORKLOADER_H
