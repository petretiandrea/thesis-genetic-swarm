include_directories(${CMAKE_SOURCE_DIR}/genetic/parallel)

add_executable(
        swarm_evolve_parallel
        main.cpp
        ParallelEvalutator.h
        BNGenomeSharedMemory.h MasterSlave.h MasterSlave.cpp ParallelEvaluator.cpp)

target_link_libraries(swarm_evolve_parallel
        rt pthread
        galib_wrapper
        utility
        evolution_loop
        genetic_core
        argos3core_simulator
        argos3plugin_simulator_entities
        argos3plugin_simulator_footbot
        ${GALIB_LIBRARIES})