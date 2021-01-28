//
// Created by andrea on 05/01/21.
//

#include "BNGenome.h"
#include <utility/Utility.h>
#include "GenomeEvalData.h"

using namespace bngenome;

GenomeInitializer<GA1DBinaryStringGenome&> bngenome::initializer(rnd::Random& rnd, float bias) {
    return [&rnd, bias](GA1DBinaryStringGenome& genome) {
        auto booleans = utility::generateBinaryVector(genome.size(), rnd, bias);
        for(int i = 0; i < genome.size(); i++) {
            genome.gene(i, booleans[i]);
        }
        // TODO: empty custom evaldata ?
    };
}

GenomeEvaluator<GA1DBinaryStringGenome&> bngenome::evaluatorByExperiment(GeneticExperimentContext& experiment) {
    return [&experiment](GA1DBinaryStringGenome& genome) {

        double performance = 0;
        double robotCount = 0;

        experiment.loop.ConfigureFromGenome(bngenome::toVector(genome));
        for(int i = 0; i < experiment.configuration.n_trials; i++) {
            experiment.loop.PrepareForTrial(i);
            experiment.simulator.Reset();
            experiment.simulator.Reset();
            experiment.simulator.Execute();

            performance += experiment.loop.Evaluate();
        }

        // TODO: add robot count to associated result
        // evaluationContext->robotCount = robotCount / (double) context->configuration.n_trials;
        return (float) (performance / (double) experiment.configuration.n_trials);
    };
}

GenomeEvaluator<vector<bool>&> bngenome::evaluatorByExperiment2(GeneticExperimentContext& experiment) {
    return [&experiment](vector<bool>& genome) {

        double performance = 0;
        double robotCount = 0;

        experiment.loop.ConfigureFromGenome(genome);
        for(int i = 0; i < experiment.configuration.n_trials; i++) {
            experiment.loop.PrepareForTrial(i);
            experiment.simulator.Reset();
            experiment.simulator.Reset();
            experiment.simulator.Execute();

            performance += experiment.loop.Evaluate();
        }

        // TODO: add robot count to associated result
        // evaluationContext->robotCount = robotCount / (double) context->configuration.n_trials;
        return (float) (performance / (double) experiment.configuration.n_trials);
    };
}

vector<bool> bngenome::toVector(const GA1DBinaryStringGenome& genome) {
    vector<bool> v(genome.size());
    for(int i = 0; i < genome.size(); i++) {
        v[i] = genome.gene(i);
    }
    return v;
}