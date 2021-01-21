//
// Created by andrea on 05/01/21.
//

#ifndef SWARM_GEN_PARALLELEVALUTATOR_H
#define SWARM_GEN_PARALLELEVALUTATOR_H

#include "MasterSlave.h"
#include "BNGenomeSharedMemory.h"
#include <vector>
#include <core/configuration/ExperimentConfiguration.h>
#include <core/configuration/ArgosExperiment.h>
#include <ga/ga.h>

using namespace std;

class ParallelEvaluator {

public:
    ParallelEvaluator(const config::ExperimentConfiguration& configuration,
                      const char* argosExperimentFilename,
                      int parallelism);

    std::function<void(GAPopulation&)> populationEvaluator();
    std::function<double(GA1DBinaryStringGenome&)> genomeEvaluator();

private:
    void evaluatePopulation(GAPopulation& population);
    [[noreturn]] void slaveBehaviour(int slaveId,
                                     const config::GeneticConfiguration &configuration,
                                     ticpp::Document& experimentXML);

private:
    int parallelism;
    BNGenomeSharedMemory memory;
    MasterSlave<BNGenomeSharedMemory> workers;
    ticpp::Document xmlExperiment;

    void prepareTaskSlave(int genomeCount);
};

#endif //SWARM_GEN_PARALLELEVALUTATOR_H
