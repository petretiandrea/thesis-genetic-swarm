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
#include <genetic/test/testing.h>
#include <utility/Utility.h>

#define CONFIG_JSON "experiments/task2.json"
#define TRAIN_EXPERIMENT "experiments/task2.argos"
#define TRAIN_STATISTICS_FOLDER "statistics/task2/"
#define TEST_EXPERIMENT "experiments/task2.argos"
#define TEST_RUN 30
#define TEST_STATISTICS_FOLDER "statistics/task2/"

int main() {

    auto userConfig = config::create_from_file(CONFIG_JSON);
    static auto statisticsBasename = PerformanceLogger::statisticsBasenameFromConfiguration(userConfig, "task2");
    auto logger = PerformanceLogger(TRAIN_STATISTICS_FOLDER, statisticsBasename);

    ParallelEvaluator parallel(userConfig, TRAIN_EXPERIMENT, 9);

    rnd::Random rnd(123);
    GA1DBinaryStringGenome genome(userConfig.genetic_config.genome_size);
    GeneticBuilder<GA1DBinaryStringGenome> evaluator(genome,
            bngenome::initializer(rnd, (float) userConfig.controller_config.bias),
            parallel.genomeEvaluator());

    auto iterator = evaluator.maximize()
            .seed(1234)
            .mutationRate(userConfig.genetic_config.mutation_probability)
            .crossoverRate(userConfig.genetic_config.crossover_probability)
            .populationSize(userConfig.genetic_config.population_size)
            .elitismReplacement(userConfig.genetic_config.replacement())
            .populationEvaluator(parallel.populationEvaluator())
            .build(userConfig.genetic_config.generations);


    cout << "Starting generations..." << endl;
    Result lastResult;
    for(auto& result : iterator) {
        cout << "Done generation " << result.generation
             << " Best fitness " << result.population->best().score()
             << endl;

        lastResult = result;
        logger.saveStatistics(*result.population);
    }

    // evaluation phase
    cout << "Evaluating phase" << endl;
    stringstream ss; ss << lastResult.population->best();
    cout << "Using best genome with score" << lastResult.population->best().score() << endl;

    testing::evaluate(ss.str(), userConfig, TEST_EXPERIMENT,string(TEST_STATISTICS_FOLDER) + "result_" + statisticsBasename, TEST_RUN);
}