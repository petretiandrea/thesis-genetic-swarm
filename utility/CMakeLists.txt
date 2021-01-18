include_directories(${CMAKE_SOURCE_DIR}/utility)

add_library(
        utility SHARED
        Utility.h Utility.cpp
        Random.h Random.cpp
        rapidcsv.h
        JsonPrettify.h
        Matrix.h)