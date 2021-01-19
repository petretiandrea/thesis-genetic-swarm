#include <iostream>

#include <argos3/core/simulator/simulator.h>
#include <core/configuration/ArgosExperiment.h>
#include <genetic/core/BNGenome.h>
#include <galib-wrapper/GeneticBuilder.h>
#include <genetic/core/PerformanceLogger.h>

using namespace std;

int main(){

    rnd::Random rnd(123);
    auto userConfig = config::create_from_file("default.json");

    static argos::CSimulator& simulator = argos::CSimulator::GetInstance();
    config::load_experiment_config(simulator, "experiments/check-arena.argos", userConfig);

    auto experiment = bngenome::create_context(userConfig.genetic_config, simulator);
    experiment.loop.GenerateRandomSpawnLocation(userConfig.genetic_config.n_trials);


    auto logger = PerformanceLogger("statistics/", "task1_" + PerformanceLogger::statisticsBasenameFromConfiguration(userConfig));

    GA1DBinaryStringGenome genome(userConfig.genetic_config.genome_size);
    GeneticBuilder<GA1DBinaryStringGenome> evaluator(
            genome,
            bngenome::initializer(rnd, (float) userConfig.controller_config.bias),
            bngenome::evaluator(experiment)
    );

    auto iterator = evaluator.maximize()
            .mutationRate(userConfig.genetic_config.mutation_probability)
            .crossoverRate(userConfig.genetic_config.crossover_probability)
            .populationSize(userConfig.genetic_config.population_size)
            .elitismReplacement(userConfig.genetic_config.replacement())
            .seed(1234)
            .build(userConfig.genetic_config.generations);

    cout << "Starting generations..." << endl;
    for(auto& result : iterator) {
        cout << "Done generation " << result.generation
            << " Best fitness " << result.population->best().fitness()
            << endl;

        logger.saveStatistics(*result.population);
    }

    simulator.Destroy();

    return 0;
}