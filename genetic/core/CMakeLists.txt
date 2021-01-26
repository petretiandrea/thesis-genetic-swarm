
add_library(genetic_core SHARED
        BNGenome.h
        BNGenome.cpp
        GenomeEvalData.h
        GenomeEvalData.cpp
        PerformanceLogger.h
        PerformanceLogger.cpp)

target_link_libraries(
        genetic_core
        utility
        loop_functions
        galib_wrapper
        ${GALIB_LIBRARIES}
)