cmake_minimum_required(VERSION 3.16)
project(swarm_gen)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

set(CMAKE_CXX_STANDARD 17)

# Set the path where CMake will find additional scripts
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Boost 1.75.0 COMPONENTS date_time json mpi)
if(BOOST_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARIES})
endif()

# Find the ARGoS package
find_package(PkgConfig)
pkg_check_modules(ARGOS REQUIRED argos3_simulator)
set(ARGOS_PREFIX ${ARGOS_PREFIX} CACHE INTERNAL "")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ARGOS_PREFIX}/share/argos3/cmake)

# Check whether all the necessary libs have been installed to compile the
# code that depends on Qt and OpenGL
include(ARGoSCheckQTOpenGL)

# Find the GSL package
find_package(GSL REQUIRED)

# Find Lua
find_package(Lua53 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS} ${LUA_INCLUDE_DIR} ${GSL_INCLUDE_DIR})

link_directories(${ARGOS_LIBRARY_DIRS})

find_package(GALIB)
if (GALIB_FOUND)
    include_directories(${GALIB_INCLUDE_DIR})
endif ()

add_subdirectory(galib-wrapper)
add_subdirectory(core)
add_subdirectory(utility)
add_subdirectory(genetic)
add_subdirectory(controller)
add_subdirectory(loop_function)
add_subdirectory(complexity)
