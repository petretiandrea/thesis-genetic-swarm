//
// Created by Andrea Petreti on 20/01/2021.
//

#ifndef SWARM_GEN_TESTING_H
#define SWARM_GEN_TESTING_H

#include <argos3/core/simulator/simulator.h>
#include <core/configuration/ArgosExperiment.h>
#include <string>

using namespace std;

namespace testing {
    void evaluate(const string& networkGenome,
                  config::ExperimentConfiguration& configuration,
                  const string& argosTestFilename,
                  const string& resultFilename,
                  int runs);

    void runTest(config::ExperimentConfiguration& configuration,
                 string& argosTestFilename,
                 string& resultFilename,
                 int runs);
}

#endif //SWARM_GEN_TESTING_H
