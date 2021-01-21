//
// Created by Andrea Petreti on 20/01/2021.
//

#ifndef SWARM_GEN_BOOLEANNETWORKSERIALIZER_H
#define SWARM_GEN_BOOLEANNETWORKSERIALIZER_H

#include <string>
#include <utility/rapidcsv.h>
#include <utility/Utility.h>

class BooleanNetworkSerializer {

public:
    static vector<bool> fromStatisticFile(const string& filename) {
        rapidcsv::Document document(filename, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(';'));

        std::cout << "parsing network from statistics file, found " << document.GetRowCount() << " rows" << endl;

        auto fitness = document.GetCell<double>("fitness", 0);
        auto booleanFunctionsString = document.GetCell<string>("genome", 0);

        std::cout << "found network with fitness " << fitness << std::endl;

        return utility::boolVectorFromString(booleanFunctionsString);
    }


};

#endif //SWARM_GEN_BOOLEANNETWORKSERIALIZER_H
