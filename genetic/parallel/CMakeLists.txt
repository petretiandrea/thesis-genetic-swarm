include_directories(${CMAKE_SOURCE_DIR}/genetic/parallel)

SET(DEPS rt pthread
        testing
        galib_wrapper
        utility
        loop_functions
        genetic_core
        argos3core_simulator
        argos3plugin_simulator_entities
        argos3plugin_simulator_footbot
        ${GALIB_LIBRARIES})

add_executable(
        swarm_evolve_parallel
        main.cpp
        ParallelEvalutator.h
        BNGenomeSharedMemory.h
        MasterSlave.h
        ParallelEvaluator.cpp)

target_link_libraries(swarm_evolve_parallel ${DEPS})


add_executable(swarm-evolve-mpi
        mpi/MPI.cpp
        mpi/MPIEvaluator.h
        mpi/MPIEvaluator.cpp
)

target_link_libraries(swarm-evolve-mpi boost_mpi boost_serialization ${DEPS})