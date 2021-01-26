//
// Created by Andrea Petreti on 20/01/2021.
//

#ifndef SWARM_GEN_BOOLEANNETWORKLOADER_H
#define SWARM_GEN_BOOLEANNETWORKLOADER_H

#include <string>
#include <utility/rapidcsv.h>
#include <utility/Utility.h>

class BooleanNetworkLoader {

public:
    static vector<bool> loadBestBooleanFunctions(const string& filename) {
        std::cout << "parsing network from statistics file..." << endl;

        auto* functions = loadBooleanFunctions(filename);

        // TODO: remove this and implement ordering
        assert(functions->size() == 1);

        auto best = functions->back();

        cout << "found network with fitness " << best.first << endl;
        return best.second;
    }


    static vector<pair<double, vector<bool>>>* loadBooleanFunctions(const string& filename) {
        rapidcsv::Document document(filename, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(';'));

        cout << "parsing multiple network from statistics file, found " << document.GetRowCount() << " rows" << endl;

        auto* data = new vector<pair<double, vector<bool>>>();
        for(int i = 0; i < document.GetRowCount(); i++) {
            auto fitness = document.GetCell<double>("fitness", i);
            auto booleanFunctionsString = document.GetCell<string>("genome", i);

            data->push_back({
                    fitness,
                    utility::boolVectorFromString(booleanFunctionsString)
            });
        }

        return data;

    }
};

#endif //SWARM_GEN_BOOLEANNETWORKLOADER_H
