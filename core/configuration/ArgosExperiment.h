//
// Created by andrea on 05/01/21.
//

#ifndef SWARM_GEN_ARGOSEXPERIMENT_H
#define SWARM_GEN_ARGOSEXPERIMENT_H

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/utility/configuration/tinyxml/ticpp.h>
#include <core/configuration/ExperimentConfiguration.h>
#include <core/configuration/ConfigurationJson.h>

using namespace argos;

bool operator!= (ticpp::Element const& elem1, ticpp::Element const& elem2);

bool operator== (ticpp::Element const& elem1, ticpp::Element const& elem2);

namespace config {

    ticpp::Document load_experiment_config(const char* experiment_filename, const struct ExperimentConfiguration& config);

    inline ticpp::Document load_experiment_config(const char* experiment_filename, const char* config_filename) {
        return config::load_experiment_config(experiment_filename, config::create_from_file(config_filename));
    }

    inline void load_experiment_config(argos::CSimulator& simulator, const char* experiment_filename, const struct ExperimentConfiguration& config) {
        auto simulatorConfig = config::load_experiment_config(experiment_filename, config);
        simulator.Load(simulatorConfig);
    }
}

#endif //SWARM_GEN_ARGOSEXPERIMENT_H
