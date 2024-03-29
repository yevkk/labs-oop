cmake_minimum_required(VERSION 3.17)
project(lab)

set(CMAKE_CXX_STANDARD 17)

set(TOP_DIR ${CMAKE_SOURCE_DIR})
set(SRC_DIR ${TOP_DIR}/src)
set(TESTS_DIR ${TOP_DIR}/tests)
set(BENCHMARK_DIR ${TOP_DIR}/benchmark)

set(LIB_NAME matrix)

set(SRC_LIST
        ${SRC_DIR}/Matrix.hpp
        ${SRC_DIR}/Matrix.hxx
        )

add_library(${LIB_NAME} INTERFACE)

target_sources(${LIB_NAME} INTERFACE ${SRC_LIST})

target_include_directories (${LIB_NAME}
        INTERFACE
        $<BUILD_INTERFACE:${SRC_DIR}/${LIB_NAME}>
        $<INSTALL_INTERFACE:${LIB_NAME}>
        )

option(ENABLE_TESTS "Build tests for project" ON)
option(ENABLE_BENCHMARK "Build benchmark for project" ON)

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(${TESTS_DIR})
endif()

if (ENABLE_BENCHMARK)
    add_subdirectory(${BENCHMARK_DIR})
endif()

# executable for quick testing in main.cpp
add_executable(main main.cpp)
target_link_libraries(main PRIVATE ${LIB_NAME})
