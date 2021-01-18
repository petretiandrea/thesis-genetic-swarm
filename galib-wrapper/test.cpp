//
// Created by andreap on 18/01/2021.
//

#include <ga/GA1DBinStrGenome.h>
#include "GeneticBuilder.h"
#include <type_traits>
#include <functional>

using namespace std;
int main() {

    GA1DBinaryStringGenome genome(10);

    srand(123);
    auto initializer = [](GA1DBinaryStringGenome& genome) {
        for(int i = 0; i < genome.size(); i++) {
            genome.gene(i, drand48() <= 0.5 ? 1 : 0);
        }
        std::cout << "uuu " << genome << std::endl;
    };

    auto evaluator = [](GA1DBinaryStringGenome& genome) {
        double count = 0.0;
        for(int i = 0; i < genome.size(); i++) { count+= genome.gene(i); }
        auto a = count / genome.size();
        std::cout << "eee " << genome << " " << a << std::endl;
        return a;
    };


    GeneticBuilder<GA1DBinaryStringGenome> builder(genome, initializer, evaluator);

    builder.populationSize(10);
    builder.mutationRate(0.05f);
    builder.crossoverRate(0.01f);
    builder.elitismReplacement(5);
    builder.maximize();

    auto iterator = builder.build(5);
    for(auto result : iterator) {
        std::cout << "Generation " << result.generation << std::endl;
        for(int i = 0; i < result.population->size(); i++) {
            auto& individual = result.population->individual(i);
            std::cout << "Individual: " << individual << " Fit " << individual.fitness() << std::endl;
        }
    }

    return 0;
}