# CMakeList.txt : CMake project for PC_assignment1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.9)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("PC_assignment1")

find_package(OpenMP)
# Add source to this project's executable.
add_executable (PC_assignment1 "PC_assignment1.cpp" "PC_assignment1.h" "support_methods.h"  "globals.h" "matrix.cpp" "globals.cpp" "benchmarking.cpp" "parallel_gauss_methods_basic.cpp" "matrix.h")

if(OpenMP_CXX_FOUND)
  target_link_libraries(${PROJECT_NAME}
    # OpenMP::OpenMP_CXX is an imported target with INTERFACE_**
    $<$<CONFIG:Release>:OpenMP::OpenMP_CXX> )
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PC_assignment1 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

