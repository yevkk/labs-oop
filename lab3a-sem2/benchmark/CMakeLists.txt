project(lab_benchmark)

set(CMAKE_BUILD_TYPE RELEASE)

add_executable(lab_benchmark benchmark.cpp)

target_link_libraries(lab_benchmark PRIVATE benchmark::benchmark ${LIB_NAME})

add_subdirectory(googletest)
add_subdirectory(benchmark)
