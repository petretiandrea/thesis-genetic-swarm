
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
        evolution_loop
        galib_wrapper
        ${GALIB_LIBRARIES}
)