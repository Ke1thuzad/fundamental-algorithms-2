add_library(lab2task2 src/main.cpp include/bigint.h)

add_executable(tests22 tests/test_bigint.cpp)
target_link_libraries(tests22 PRIVATE lab2task2 GTest::gtest_main)

add_test(NAME Test22 COMMAND tests22)