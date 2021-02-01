
add_executable(
        complexity
        main.cpp
        Complexity.h
)

target_link_libraries(
        complexity
        rt pthread
        core
        testing
        utility
        loop_functions
        argos3core_simulator
        argos3plugin_simulator_entities
        argos3plugin_simulator_footbot)