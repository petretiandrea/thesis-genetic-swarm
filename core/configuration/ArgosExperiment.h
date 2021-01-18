//
// Created by andrea on 05/01/21.
//

#ifndef SWARM_GEN_ARGOSEXPERIMENT_H
#define SWARM_GEN_ARGOSEXPERIMENT_H

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/configuration/tinyxml/ticpp.h>
#include <core/configuration/ExperimentConfiguration.h>
#include "ConfigurationJson.h"

#define KEY_FOOTBOT_NUMBER "n_footbot"

using namespace argos;

bool operator!= (ticpp::Element const& elem1, ticpp::Element const& elem2) {
    return elem1.operator!=(elem2);
}

bool operator== (ticpp::Element const& elem1, ticpp::Element const& elem2) {
    return elem1.operator==(elem2);
}

namespace config {

    inline ticpp::Document load_experiment_config(const char* experiment_filename, const struct ExperimentConfiguration& config) {
        ticpp::Document document;
        document.LoadFile(experiment_filename);

        TConfigurationNode& root = *document.FirstChildElement();

        // inject config to loop function
        auto loop_fun = argos::GetNode(root, "loop_functions");
        if(loop_fun.GetAttribute("label") == "evolution_loop") {
            loop_fun.SetAttribute(KEY_FOOTBOT_NUMBER, config.footbot_size);
        }

        // inject config to controller
        auto controllers = argos::GetNode(root, "controllers");
        TConfigurationNodeIterator it;
        for(it = it.begin(&controllers); it != it.end(); ++it) {
            std::string controller_type;
            it->GetValue(&controller_type);
            if(controller_type == "bn_controller"){
                auto params = it->FirstChildElement("params");
                params->SetAttribute(key::NODES, config.controller_config.nodes);
                params->SetAttribute(key::BIAS, config.controller_config.bias);
                params->SetAttribute(key::INPUT_FOR_NODE, config.controller_config.input_for_node);
                params->SetAttribute(key::PROXIMITY_THRESHOLD, config.controller_config.proximity_threshold);
                params->SetAttribute(key::MOTOR_GROUND_THRESHOLD, config.controller_config.motor_ground_threshold);
                params->SetAttribute(key::CONSTANT_SPEED, config.controller_config.constant_speed);
            }
        }

        return document;
    }

    inline ticpp::Document load_experiment_config(const char* experiment_filename, const char* config_filename) {
        return config::load_experiment_config(experiment_filename, config::create_from_file(config_filename));
    }

    inline void load_experiment_config(argos::CSimulator& simulator, const char* experiment_filename, const struct ExperimentConfiguration& config) {
        auto simulatorConfig = config::load_experiment_config(experiment_filename, config);
        simulator.Load(simulatorConfig);
    }
}

#endif //SWARM_GEN_ARGOSEXPERIMENT_H
