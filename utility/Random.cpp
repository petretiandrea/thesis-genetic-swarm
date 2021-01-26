//
// Created by Andrea Petreti on 04/01/2021.
//
#include "Random.h"
#include <chrono>

using namespace rnd;

std::uniform_real_distribution<double> Random::distribution = std::uniform_real_distribution<double>(0.0, 1.0);

Random::Random() : generator(std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count())) { }
Random::Random(unsigned int seed) : generator(std::mt19937(seed)) {}

double Random::next() {
    return distribution(generator);
}

int Random::nextInt(int min, int max) {
    return (int) floor(next() * (max - min + 1)) + min;
}