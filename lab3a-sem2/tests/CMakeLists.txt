project(tests)

SET(SRC_LIST
        main.cpp
        MatrixTest.cpp
        )

add_executable(tests ${SRC_LIST})

target_link_libraries(tests PRIVATE ${LIB_NAME})

add_test(NAME TestAll COMMAND tests)