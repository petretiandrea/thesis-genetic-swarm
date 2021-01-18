//
// Created by Andrea Petreti on 04/01/2021.
//

#ifndef SWARM_GEN_RANDOM_H
#define SWARM_GEN_RANDOM_H

#include <random>

namespace rnd {

    typedef std::mt19937 RNG;

    class Random {

    public:
        Random();
        explicit Random(unsigned int seed);

        double next();
        int nextInt(int min, int max);

    private:
        static std::uniform_real_distribution<double> distribution;
        RNG generator;
    };
}

#endif //SWARM_GEN_RANDOM_H
