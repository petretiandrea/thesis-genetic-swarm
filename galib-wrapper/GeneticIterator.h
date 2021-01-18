//
// Created by andreap on 18/01/2021.
//

#ifndef SWARM_GEN_GENETICITERATOR_H
#define SWARM_GEN_GENETICITERATOR_H

#include <ga/ga.h>
#include <iterator>

struct Result {
    int generation;
    const GAPopulation* population;
public:
    Result() : generation(0), population(nullptr) {};
};

class GeneticIterator : public std::iterator<std::input_iterator_tag,
        Result, ptrdiff_t, const Result*, const Result&> {

private:
    GASteadyStateGA* algorithm;
    Result result;
    int seed;

public:
    explicit GeneticIterator() : algorithm(nullptr), seed(-1) {};

    explicit GeneticIterator(GASteadyStateGA& algorithm, int generations, int seed) :
        algorithm(&algorithm),
        seed(seed) {

        this->algorithm->nGenerations(generations);
        this->result.generation = this->algorithm->generation();
        this->result.population = &this->algorithm->population();
    }

    reference operator*() {
        return result;
    }

    pointer operator->() {
        return &result;
    }

    GeneticIterator& operator++() {
        if (!algorithm->done()) {
            algorithm->step();
            result.generation = algorithm->generation();
            result.population = &algorithm->population();
        } else {
            *this = GeneticIterator();
        }
        return *this;
    }

    friend bool operator==(const GeneticIterator& self, const GeneticIterator& other) {
        return self.algorithm == other.algorithm;
    }

    friend bool operator!= (const GeneticIterator& self, const GeneticIterator& other) {
        return !(self == other);
    }

    inline GeneticIterator& begin() {
        algorithm->initialize(seed);
        return *this;
    }

    inline GeneticIterator end() {
        return GeneticIterator();
    }
};


#endif //SWARM_GEN_GENETICITERATOR_H
