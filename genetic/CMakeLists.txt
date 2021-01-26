include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(sequential)
add_subdirectory(parallel)

set(COMMON_DEPS rt pthread galib_wrapper
        utility
        loop_functions
        genetic_core
        argos3core_simulator
        argos3plugin_simulator_entities
        argos3plugin_simulator_footbot
        ${GALIB_LIBRARIES})

# testing as shared lib
add_library(testing STATIC test/testing.h test/testing.cpp)

add_executable(test test/main.cpp)
target_link_libraries(test testing ${COMMON_DEPS})


add_executable(visualize visualize/visualize.cpp)
target_link_libraries(visualize ${COMMON_DEPS})

add_executable(scala scala.cpp)
target_link_libraries(scala ${COMMON_DEPS})