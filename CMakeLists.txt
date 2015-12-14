cmake_minimum_required(VERSION 2.6)
project(TDP4)

file(GLOB_RECURSE util "src/util.*")

file(GLOB_RECURSE util "src/fox.*")

file(GLOB_RECURSE util "src/dispatch.*")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR)/build")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

find_package(MPI)
message("Importing MPI : ${MPI_LIBRARIES}")

set(PROJECT_INCLUDE_DIR "src")
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${MPI_INCLUDE_PATH}")

add_executable("test" src/test.c ${utils} ${fox} ${dispatch})

target_link_libraries("test" ${BLAS_LIBRARIES} ${MPI_LIBRARIES})

add_executable("benchmark" src/benchmark.c ${utils} ${fox} ${dispatch})

target_link_libraries("benchmark" ${MPI_LIBRARIES})

add_executable("app" src/app.c ${utils} ${fox} ${dispatch})

target_link_libraries("app" ${MPI_LIBRARIES})
