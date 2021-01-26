include_directories(${CMAKE_SOURCE_DIR}/core)

add_library(core SHARED
        BooleanNetwork.h BooleanNetwork.cpp
        configuration/ExperimentConfiguration.h
        configuration/ConfigurationJson.h
        configuration/ArgosExperiment.h
        configuration/config.h
        configuration/ArgosExperiment.cpp
        configuration/ConfigurationJson.cpp BooleanNetworkLoader.h)


