//
// Created by andrea on 05/01/21.
//

#include "BNGenomeSharedMemory.h"
#include <core/configuration/ArgosExperiment.h>
#include <ga/ga.h>
#include <galib-wrapper/GeneticBuilder.h>
#include <genetic/core/BNGenome.h>
#include "ParallelEvalutator.h"

void saveIntoLog(const GAPopulation* population);

int main() {

    auto userConfig = config::create_from_file("default.json");

    ParallelEvaluator parallel(userConfig, "test.argos", 2);

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
             << " Best fitness " << result.population->best().fitness()
             << endl;

        saveIntoLog(result.population);
    }
}


void saveIntoLog(const GAPopulation* population) {
    static double best_score = 0;
    static GAGenome* best_genome = nullptr;

    cout << "Saving population info " << endl;
    for(int i = 0; i < population->size(); i++) {
        auto& individual = population->individual(i);

        cout << "Genome " << individual << endl;
        if(individual.score() > best_score) {
            best_genome = &individual;
            best_score = best_genome->score();
            cout << "Best score: " << best_score << " booleans: " << *best_genome << endl;
            //logger.saveGenomeAsBest(*best_genome);
        }
    }
}