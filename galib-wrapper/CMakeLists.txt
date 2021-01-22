
add_library(galib_wrapper SHARED GeneticBuilder.h GeneticIterator.h)

target_link_libraries(
        galib_wrapper
        ${GALIB_LIBRARIES}
)