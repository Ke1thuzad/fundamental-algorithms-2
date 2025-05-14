add_library(lab2task1 src/main.cpp include/bigint.h)

add_executable(tests21 tests/test_bigint.cpp)
target_link_libraries(tests21 PRIVATE lab2task1 GTest::gtest_main)

add_test(NAME Test21 COMMAND tests21)