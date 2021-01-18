//
// Created by andrea on 05/01/21.
//

#ifndef SWARM_GEN_BNGENOME_H
#define SWARM_GEN_BNGENOME_H

#include <core/configuration/ExperimentConfiguration.h>
#include <ga/ga.h>
#include <functional>
#include <utility>
#include <loop_function/evolution/evolution_loop.h>
#include <galib-wrapper/GeneticBuilder.h>

using namespace std;

namespace bngenome {

    struct GeneticExperimentContext {
        config::GeneticConfiguration& configuration;
        argos::CSimulator& simulator;
        EvolutionLoop& loop;
    };

    inline GeneticExperimentContext create_context(config::GeneticConfiguration& config, argos::CSimulator& simulator) {
        return {
            .configuration = config,
            .simulator = simulator,
            .loop = dynamic_cast<EvolutionLoop&>(simulator.GetLoopFunctions())
        };
    }

    GenomeInitializer<GA1DBinaryStringGenome&> initializer(rnd::Random& rnd, float bias);
    GenomeEvaluator<GA1DBinaryStringGenome> evaluator(GeneticExperimentContext& experiment);
    vector<bool> toVector(const GA1DBinaryStringGenome& genome);
}

#endif //SWARM_GEN_BNGENOME_H
