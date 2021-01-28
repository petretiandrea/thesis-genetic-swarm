//
// Created by Andrea Petreti on 28/01/2021.
//

#ifndef SWARM_GEN_SOLUTIONLOADER_H
#define SWARM_GEN_SOLUTIONLOADER_H

#include <string>
#include <utility/rapidcsv.h>
#include <utility/Utility.h>

struct Solution {
    int generation;
    double fitness;
    vector<bool> solution;

    bool operator ==(const Solution & obj) const
    {
        if (this == &obj || this->solution == obj.solution)
            return true;
        else
            return false;
    }
};

class SolutionLoader {
public:
    static inline vector<Solution>* loadSolutionsLog(const string& filename) {
        rapidcsv::Document document(filename, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(';'));

        cout << "parsing multiple network from statistics file, found " << document.GetRowCount() << " rows" << endl;

        auto* data = new vector<Solution>();
        for(int i = 0; i < document.GetRowCount(); i++) {
            auto generation = document.GetCell<int>("generation", i);
            auto fitness = document.GetCell<double>("fitness", i);
            auto booleanFunctionsString = document.GetCell<string>("genome", i);

            data->push_back({
                .generation = generation,
                .fitness = fitness,
                .solution = utility::boolVectorFromString(booleanFunctionsString)
            });
        }

        return data;

    }
};

#endif //SWARM_GEN_SOLUTIONLOADER_H
