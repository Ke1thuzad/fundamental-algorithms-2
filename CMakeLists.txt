cmake_minimum_required(VERSION 3.26)
project(fundamental_algorithms_2 CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/utility)

file(GLOB LIB_SOURCES "${CMAKE_SOURCE_DIR}/utility/*.c" "${CMAKE_SOURCE_DIR}/utility/arrays/*.c")


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COVERAGE_FLAGS
            --coverage
            -fprofile-arcs
            -ftest-coverage
    )
endif ()

add_compile_options(-Wall -Wextra -Werror -Wpedantic ${COVERAGE_FLAGS})
add_link_options(${COVERAGE_FLAGS})

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_program(LCOV lcov)
find_program(GENHTML genhtml)

add_subdirectory(lab1)
