//
// Created by andrea on 04/01/21.
//

#ifndef SWARM_GEN_EXPERIMENTCONFIGURATION_H
#define SWARM_GEN_EXPERIMENTCONFIGURATION_H

#include <string>

namespace config {

    struct GeneticConfiguration {
        int n_trials;
        int genome_size;
        int generations;
        double mutation_probability;
        double crossover_probability;
        int population_size;
        int elitism;

        int replacement() const { return (int)(population_size - (population_size / (float) elitism)); }
    };

    struct ControllerConfiguration {
        int nodes;
        int input_for_node;
        double bias;
        double proximity_threshold;
        double motor_ground_threshold;
        double constant_speed;
    };

    struct ExperimentConfiguration {
        int footbot_size;
        ControllerConfiguration controller_config;
        GeneticConfiguration genetic_config;
    };
}



#endif //SWARM_GEN_EXPERIMENTCONFIGURATION_H
