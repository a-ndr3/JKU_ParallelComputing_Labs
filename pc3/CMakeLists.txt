cmake_minimum_required(VERSION 3.9)
project(pc3)

set(CMAKE_CXX_STANDARD 17)

#set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -O3 -I ${MPI_ROOT}/Include -L ${MPI_ROOT}/Lib/x64/ -lmpi")
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

# Set the path to Microsoft MPI installation directory
set(MPI_ROOT "C:/Program Files (x86)/Microsoft SDKs/MPI")

# Include MPI headers
include_directories(${MPI_ROOT}/Include)

add_executable(pc3 main.cpp benchmarking.cpp support_methods.h globals.cpp logger.cpp parallel_gauss_methods_basic.cpp flatmatrix.cpp flatmatrix.h gauss_methods_flat.cpp Logger2.cpp)

# Link against MPI libraries
target_link_libraries(pc3 ${MPI_ROOT}/Lib/x64/msmpi.lib)


