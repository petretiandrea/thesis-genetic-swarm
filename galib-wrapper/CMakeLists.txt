
add_library(galib_wrapper SHARED GeneticBuilder.h GeneticIterator.h)

target_link_libraries(
        galib_wrapper
        ${GALIB_LIBRARIES}
)

add_executable(test_wrapper test.cpp GeneticIterator.h)
target_link_libraries(
        test_wrapper
        galib_wrapper
        ${GALIB_LIBRARIES})