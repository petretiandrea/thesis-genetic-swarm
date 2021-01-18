include_directories(${CMAKE_SOURCE_DIR}/core)

add_library(core SHARED
        BooleanNetwork.h BooleanNetwork.cpp
        configuration/ExperimentConfiguration.h
        configuration/ConfigurationJson.h
        configuration/ArgosExperiment.h
        configuration/config.h
        Logger.h)

#target_link_directories()
