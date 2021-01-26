//
// Created by andreap on 18/01/2021.
//

#ifndef SWARM_GEN_GENETICBUILDER_H
#define SWARM_GEN_GENETICBUILDER_H

#include <ga/ga.h>
#include <ga/GAGenome.h>
#include <functional>
#include <type_traits>
#include <utility>
#include "GeneticIterator.h"
#include <boost/optional.hpp>

using namespace std;

template<typename G> using GenomeInitializer = function<void(G&)>;
template<typename G> using GenomeEvaluator = function<double(G&)>;
using PopulationEvaluator = function<void(GAPopulation&)>;

template<typename G>
class GeneticBuilder {

    static_assert(std::is_base_of<GAGenome, G>::value, "G must inherit from GAGenome");

public:
    static const int DEFAULT_SEED = 123;
    const GAScalingScheme& DEFAULT_FIT_SCALING = GANoScaling();

public:
    GeneticBuilder(G& genome,
                   GenomeInitializer<G> genomeInitializer,
                   GenomeEvaluator<G> genomeEvaluator);

    GeneticBuilder<G>& populationSize(int size);
    GeneticBuilder<G>& mutationRate(double mutationRate);
    GeneticBuilder<G>& crossoverRate(double crossoverRate);
    GeneticBuilder<G>& elitismReplacement(int replacement);
    GeneticBuilder<G>& selector(GASelectionScheme& selector);
    GeneticBuilder<G>& maximize();
    GeneticBuilder<G>& minimize();
    GeneticBuilder<G>& populationEvaluator(PopulationEvaluator f);
    GeneticBuilder<G>& fitnessScaling(const GAScalingScheme& scaling);
    GeneticBuilder<G>& seed(int seed);
    GeneticIterator build(int generations);

private:
    GASteadyStateGA* algorithm;
    G& genomeTemplate;
    GenomeInitializer<G> initializer;
    GenomeEvaluator<G> genomeEvaluator;
    boost::optional<PopulationEvaluator*> populationEval;
    int currentSeed = GeneticBuilder::DEFAULT_SEED;

private:
    double evaluateGenome(G& genome);
    void evaluatePopulation(GAPopulation& population);


// statics wrapper for maintain 'this' context
private:
    static void genome_initializer(GAGenome& genome);
    static float genome_evaluator(GAGenome& genome);
    static void population_evaluator(GAPopulation& population);
};

template<typename G>
GeneticBuilder<G>::GeneticBuilder(G &genome,
                                  function<void(G&)> genomeInitializer,
                                  function<double(G&)> genomeEvaluator) :
                          genomeTemplate(genome),
                          algorithm(nullptr),
                          initializer(genomeInitializer),
                          genomeEvaluator(genomeEvaluator),
                          populationEval() {

        genomeTemplate.userData(this);
        genomeTemplate.initializer(genome_initializer);
        genomeTemplate.evaluator(genome_evaluator);

        algorithm = new GASteadyStateGA(genomeTemplate);
        algorithm->scaling(DEFAULT_FIT_SCALING);
        algorithm->nBestGenomes(0);
        algorithm->flushFrequency(0);
}

template<typename G>
GeneticBuilder<G> &GeneticBuilder<G>::populationSize(int size) {
    algorithm->populationSize(size);
    return *this;
}

template<typename G>
GeneticBuilder<G> &GeneticBuilder<G>::mutationRate(double mutationRate) {
    algorithm->pMutation((float) mutationRate);
    return *this;
}

template<typename G>
GeneticBuilder<G> &GeneticBuilder<G>::crossoverRate(double crossoverRate) {
    algorithm->pCrossover((float) crossoverRate);
    return *this;
}

template<typename G>
GeneticBuilder<G> &GeneticBuilder<G>::elitismReplacement(int replacement) {
    algorithm->nReplacement(replacement);
    return *this;
}

template<typename G>
GeneticBuilder<G>& GeneticBuilder<G>::selector(GASelectionScheme& selector) {
    algorithm->selector(selector);
    return *this;
}

template<typename G>
GeneticBuilder<G> &GeneticBuilder<G>::maximize() {
    algorithm->maximize();
    return *this;
}

template<typename G>
GeneticBuilder<G> &GeneticBuilder<G>::minimize() {
    algorithm->minimize();
    return *this;
}

template<typename G>
GeneticBuilder<G> &GeneticBuilder<G>::populationEvaluator(PopulationEvaluator f) {
    populationEval = &f;
    auto& currentPop = const_cast<GAPopulation&>(algorithm->population());
    currentPop.evaluator(population_evaluator);
    currentPop.userData(this);
    return *this;
}

template<typename G>
double GeneticBuilder<G>::evaluateGenome(G &genome) {
    return genomeEvaluator(genome);
}

template<typename G>
void GeneticBuilder<G>::evaluatePopulation(GAPopulation &population) {
    if(populationEval.has_value()) {
        populationEval.value()->operator()(population);
    }
}

template<typename G>
GeneticBuilder<G> &GeneticBuilder<G>::seed(int seed) {
    currentSeed = seed;
    return *this;
}

template<typename G>
GeneticIterator GeneticBuilder<G>::build(int generations) {
    return GeneticIterator(*algorithm, generations, currentSeed);
}

template<typename G>
GeneticBuilder<G> &GeneticBuilder<G>::fitnessScaling(const GAScalingScheme& scaling) {
    algorithm->scaling(scaling);
    return *this;
}

// static wrappers
template<typename G>
void GeneticBuilder<G>::genome_initializer(GAGenome &genome) {
    auto* self = static_cast<GeneticBuilder<G>*>(genome.userData());
    auto& genomeTemp = dynamic_cast<G&>(genome);
    self->template initializer(genomeTemp);
}

template<typename G>
float GeneticBuilder<G>::genome_evaluator(GAGenome &genome) {
    auto* self = static_cast<GeneticBuilder<G>*>(genome.userData());
    auto& genomeTemp = dynamic_cast<G&>(genome);
    return self->template evaluateGenome(genomeTemp);
}

template<typename G>
void GeneticBuilder<G>::population_evaluator(GAPopulation &population) {
    auto* self = static_cast<GeneticBuilder<G>*>(population.userData());
    self->template evaluatePopulation(population);
}

#endif //SWARM_GEN_GENETICBUILDER_H
