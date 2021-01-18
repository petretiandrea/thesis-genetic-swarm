//
// Created by andrea on 05/01/21.
//

#ifndef SWARM_GEN_CONFIG_H
#define SWARM_GEN_CONFIG_H

namespace config {
    namespace key {

        //static const char* = "";
        static const char* FOOTBOOT_SIZE = "footbot_size";
        static const char* N_TRIALS = "n_trials";
        static const char* GENOME_SIZE = "genome_size";
        static const char* GENERATIONS = "generations";
        static const char* MUTATION_PROB = "mutation_probability";
        static const char* CROSSOVER_PROB = "crossover_probability";
        static const char* POPULATION_SIZE = "population_size";
        static const char* ELITISM = "elitism";

        static const char* BOOLEAN_NETWORK_FILE = "network";

        static const char* NODES = "nodes";
        static const char* BIAS = "bias";
        static const char* INPUT_FOR_NODE = "input_for_node";
        static const char* PROXIMITY_THRESHOLD = "proximity_threshold";
        static const char* MOTOR_GROUND_THRESHOLD = "motor_ground_threshold";
        static const char* CONSTANT_SPEED = "constant_speed";
    }
}

#endif //SWARM_GEN_CONFIG_H
