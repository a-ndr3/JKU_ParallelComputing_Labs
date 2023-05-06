cmake_minimum_required(VERSION 3.25)
project(ParallelCompEx2_check C)

set(CMAKE_C_STANDARD 23)

project(libcwd
        VERSION 1.1.3
        LANGUAGES CXX C
        )

include(CheckIncludeFile)
check_include_file("sys/resource.h" HAVE_SYS_RESOURCE_H)

include_directories("C:/Users/abloh/Desktop/include")

find_library(PTHREADS_LIB
        NAMES pthreadsGC2 pthreadVC2
        HINTS "C:/Users/abloh/Desktop/libs/lib/x64/")

if(NOT PTHREADS_LIB)
    message(FATAL_ERROR "Pthreads library not found")
endif()

##set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -O3 -DNDEBUG") -pthread -ggdb3 -g
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -ggdb3 -pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(ParallelCompEx2_check main.c)
target_link_libraries(ParallelCompEx2_check ${PTHREADS_LIB})