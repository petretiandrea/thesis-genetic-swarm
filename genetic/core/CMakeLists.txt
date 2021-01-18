
add_library(genetic_core STATIC
        BNGenome.h
        BNGenome.cpp
        GenomeEvalData.h
        GenomeEvalData.cpp
        GeneticEvaluator.h)

target_link_libraries(
        genetic_core
        utility
        evolution_loop
        galib_wrapper
        ${GALIB_LIBRARIES}
)