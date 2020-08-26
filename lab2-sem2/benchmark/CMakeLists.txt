project(lab_benchmark)

add_executable(lab_benchmark benchmark.cpp)

#PRIVATE ${LIB_NAME}
target_link_libraries(lab_benchmark benchmark::benchmark)

add_subdirectory(benchmark)



