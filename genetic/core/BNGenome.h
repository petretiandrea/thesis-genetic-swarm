//
// Created by andrea on 05/01/21.
//

#ifndef SWARM_GEN_BNGENOME_H
#define SWARM_GEN_BNGENOME_H

#include <core/configuration/ExperimentConfiguration.h>
#include <ga/ga.h>
#include <functional>
#include <utility>
#include <loop_function/evolution/BaseLoop.h>
#include <galib-wrapper/GeneticBuilder.h>

using namespace std;

namespace bngenome {

    struct GeneticExperimentContext {
        const config::GeneticConfiguration& configuration;
        argos::CSimulator& simulator;
        BaseLoop& loop;
    };

    inline GeneticExperimentContext create_context(const config::GeneticConfiguration& config, argos::CSimulator& simulator) {
        return {
            .configuration = config,
            .simulator = simulator,
            .loop = dynamic_cast<BaseLoop&>(simulator.GetLoopFunctions())
        };
    }

    GenomeInitializer<GA1DBinaryStringGenome&> initializer(rnd::Random& rnd, float bias);
    GenomeEvaluator<GA1DBinaryStringGenome> evaluatorByExperiment(GeneticExperimentContext& experiment);
    GenomeEvaluator<vector<bool>&> evaluatorByExperiment2(GeneticExperimentContext& experiment);
    vector<bool> toVector(const GA1DBinaryStringGenome& genome);
}

#endif //SWARM_GEN_BNGENOME_H
