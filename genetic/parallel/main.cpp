//
// Created by andrea on 05/01/21.
//

#include "BNGenomeSharedMemory.h"
#include <core/configuration/ArgosExperiment.h>
#include <ga/ga.h>
#include <galib-wrapper/GeneticBuilder.h>
#include <genetic/core/BNGenome.h>
#include "ParallelEvalutator.h"
#include <genetic/core/PerformanceLogger.h>

int main() {

    auto userConfig = config::create_from_file("experiments/task1.json");
    auto logger = PerformanceLogger("statistics/",
                                    "task1_" + PerformanceLogger::statisticsBasenameFromConfiguration(userConfig));

    ParallelEvaluator parallel(userConfig, "experiments/task1.argos", 10);

    rnd::Random rnd(123);
    GA1DBinaryStringGenome genome(userConfig.genetic_config.genome_size);
    GeneticBuilder<GA1DBinaryStringGenome> evaluator(
            genome,
            bngenome::initializer(rnd, (float) userConfig.controller_config.bias),
            [](GA1DBinaryStringGenome& g) { return g.score(); }
    );

    auto iterator = evaluator.maximize()
            .mutationRate(userConfig.genetic_config.mutation_probability)
            .crossoverRate(userConfig.genetic_config.crossover_probability)
            .populationSize(userConfig.genetic_config.population_size)
            .elitismReplacement(userConfig.genetic_config.replacement())
            .seed(1234)
            .populationEvaluator(parallel.populationEvaluator())
            .build(userConfig.genetic_config.generations);


    cout << "Starting generations..." << endl;
    for(auto& result : iterator) {
        cout << "Done generation " << result.generation
             << " Best fitness " << result.population->best().score()
             << endl;

        logger.saveStatistics(*result.population);
    }
}