//
// Created by andrea on 04/01/21.
//

#ifndef SWARM_GEN_CONFIGURATIONJSON_H
#define SWARM_GEN_CONFIGURATIONJSON_H

#include <iostream>
#include <fstream>
#include <boost/json.hpp>
#include <utility/JsonPrettify.h>
#include <core/configuration/config.h>
#include <core/configuration/ExperimentConfiguration.h>

namespace json = boost::json;

namespace config {

    json::value create_empty_template();
    void create_empty_template(const char* filename);

    ExperimentConfiguration create_from_file(const char* filename);
}

namespace config {
/** Object to json conversion */

    void tag_invoke(json::value_from_tag, json::value& jv, ControllerConfiguration const& c);
    void tag_invoke(json::value_from_tag, json::value& jv, GeneticConfiguration const& c);
    void tag_invoke(json::value_from_tag, json::value &jv, config::ExperimentConfiguration const &c);

    template<class T>
    void extract(json::object const &obj, T &t, json::string_view key) {
        t = json::value_to<T>(obj.at(key));
    }

    /** Json to object conversion */
    GeneticConfiguration tag_invoke(json::value_to_tag<GeneticConfiguration>, json::value const& jv);

    ControllerConfiguration tag_invoke(json::value_to_tag<ControllerConfiguration>, json::value const& jv);

    ExperimentConfiguration tag_invoke(json::value_to_tag<ExperimentConfiguration>, json::value const &jv);
}

#endif //SWARM_GEN_CONFIGURATIONJSON_H
