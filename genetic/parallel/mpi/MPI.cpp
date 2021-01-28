//
// Created by Andrea Petreti on 27/01/2021.
//

#ifndef SWARM_GEN_MPI_H
#define SWARM_GEN_MPI_H


#include <iostream>
#include <string>
#include <boost/mpi.hpp>
#include <core/configuration/ArgosExperiment.h>
#include <ga/ga.h>
#include <galib-wrapper/GeneticBuilder.h>
#include <genetic/core/BNGenome.h>
#include <genetic/core/PerformanceLogger.h>
#include <genetic/test/testing.h>
#include <utility/Utility.h>
#include <constants.h>
#include "MPIEvaluator.h"


#define CONFIG_JSON "experiments/task4/config.json"
#define TRAIN_EXPERIMENT "experiments/task4/task4.argos"
#define TRAIN_STATISTICS_FOLDER "statistics/task4/"
#define TEST_EXPERIMENT "experiments/task4/task4-test.argos"
#define TEST_STATISTICS_FOLDER "statistics/task4/"
#define TEST_RUN 30

using namespace std;
namespace mpi = boost::mpi;

int main()
{
    mpi::environment env;
    mpi::communicator world;


    auto userConfig = config::create_from_file(CONFIG_JSON);
    static auto statisticsBasename = PerformanceLogger::statisticsBasenameFromConfiguration(userConfig, "task4");
    auto logger = PerformanceLogger(TRAIN_STATISTICS_FOLDER, statisticsBasename);

    MPIEvaluator eval(world);

    if(world.rank() == 0) {
        // make genetics

        rnd::Random rnd(constants::RANDOM_SEED);
        GA1DBinaryStringGenome genome(userConfig.genetic_config.genome_size);
        GeneticBuilder<GA1DBinaryStringGenome> evaluator(genome,
                                                         bngenome::initializer(rnd, (float) userConfig.controller_config.bias),
                                                         eval.genomeEvaluator());

        auto iterator = evaluator.maximize()
                .seed(1234)
                .mutationRate(userConfig.genetic_config.mutation_probability)
                .crossoverRate(userConfig.genetic_config.crossover_probability)
                .populationSize(userConfig.genetic_config.population_size)
                .elitismReplacement(userConfig.genetic_config.replacement())
                .populationEvaluator(eval.populationEvaluator())
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

    } else {
        auto xml = config::load_experiment_config(TRAIN_EXPERIMENT, userConfig);
        eval.slaveBehaviour(userConfig.genetic_config, xml);
    }

    return 0;
}
#endif //SWARM_GEN_MPI_H
