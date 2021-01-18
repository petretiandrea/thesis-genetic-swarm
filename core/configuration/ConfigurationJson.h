//
// Created by andrea on 04/01/21.
//

#ifndef SWARM_GEN_CONFIGURATIONJSON_H
#define SWARM_GEN_CONFIGURATIONJSON_H

#include "ExperimentConfiguration.h"
#include <boost/json/src.hpp>
#include <utility/JsonPrettify.h>
#include <core/configuration/config.h>

namespace json = boost::json;

namespace config {

    inline json::value create_empty_template() {
        static ExperimentConfiguration config {};
        return json::value_from(config);
    }

    inline void create_empty_template(const char* filename) {
        std::ofstream file(filename);
        if(file) {
            pretty_print(file, create_empty_template());
        }
    }

    inline ExperimentConfiguration create_from_file(const char* filename) {
        std::ifstream file(filename);
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return json::value_to<ExperimentConfiguration>(json::parse(content));
    }
}

    namespace config {
    /** Object to json conversion */

    void tag_invoke(json::value_from_tag, json::value& jv, ControllerConfiguration const& c) {
        jv = {
                { key::NODES, c.nodes },
                { key::INPUT_FOR_NODE, c.input_for_node },
                { key::BIAS, c.bias },
                { key::PROXIMITY_THRESHOLD, c.proximity_threshold },
                { key::MOTOR_GROUND_THRESHOLD, c.motor_ground_threshold },
                { key::CONSTANT_SPEED, c.constant_speed }
        };
    }

    void tag_invoke(json::value_from_tag, json::value& jv, GeneticConfiguration const& c) {
        jv = {
                { key::N_TRIALS, c.n_trials },
                { key::GENOME_SIZE, c.genome_size },
                { key::GENERATIONS, c.generations },
                { key::MUTATION_PROB, c.mutation_probability },
                { key::CROSSOVER_PROB, c.crossover_probability },
                { key::POPULATION_SIZE, c.population_size },
                { key::ELITISM, c.elitism }
        };
    }

    void tag_invoke(json::value_from_tag, json::value &jv, config::ExperimentConfiguration const &c) {
        jv = {
                { key::FOOTBOOT_SIZE, c.footbot_size},
                { "controller_config", json::value_from(c.controller_config) },
                { "genetic_config", json::value_from(c.genetic_config) }
        };
    }

    template<class T>
    void extract(json::object const &obj, T &t, json::string_view key) {
        t = json::value_to<T>(obj.at(key));
    }

    /** Json to object conversion */
    GeneticConfiguration tag_invoke(json::value_to_tag<GeneticConfiguration>, json::value const& jv)
    {
        json::object const& obj = jv.as_object();
        GeneticConfiguration c;
        extract(obj, c.n_trials, key::N_TRIALS);
        extract(obj, c.genome_size, key::GENOME_SIZE);
        extract(obj, c.generations, key::GENERATIONS);
        extract(obj, c.mutation_probability, key::MUTATION_PROB);
        extract(obj, c.crossover_probability, key::CROSSOVER_PROB);
        extract(obj, c.population_size, key::POPULATION_SIZE);
        extract(obj, c.elitism, key::ELITISM);
        return c;
    }

    ControllerConfiguration tag_invoke(json::value_to_tag<ControllerConfiguration>, json::value const& jv)
    {
        json::object const& obj = jv.as_object();
        ControllerConfiguration c {};
        extract(obj, c.nodes, key::NODES);
        extract(obj, c.input_for_node, key::INPUT_FOR_NODE);
        extract(obj, c.bias, key::BIAS);
        extract(obj, c.proximity_threshold, key::PROXIMITY_THRESHOLD);
        extract(obj, c.motor_ground_threshold,  key::MOTOR_GROUND_THRESHOLD);
        extract(obj, c.constant_speed, key::CONSTANT_SPEED);
        return c;
    }

    ExperimentConfiguration tag_invoke(json::value_to_tag<ExperimentConfiguration>, json::value const &jv) {
        json::object const &obj = jv.as_object();
        ExperimentConfiguration c;
        extract(obj, c.footbot_size, key::FOOTBOOT_SIZE);
        extract(obj, c.controller_config, "controller_config");
        extract(obj, c.genetic_config, "genetic_config");
        return c;
    }

}


#endif //SWARM_GEN_CONFIGURATIONJSON_H
