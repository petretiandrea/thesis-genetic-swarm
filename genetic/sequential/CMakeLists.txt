include_directories(${CMAKE_SOURCE_DIR}/genetic/sequential)

add_executable(
        swarm_evolve
        main.cpp
)

target_link_libraries(swarm_evolve
        rt pthread
        galib_wrapper
        utility
        evolution_loop
        genetic_core
        argos3core_simulator
        argos3plugin_simulator_entities
        argos3plugin_simulator_footbot
        ${GALIB_LIBRARIES})