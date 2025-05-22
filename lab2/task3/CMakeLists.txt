add_library(lab2task3 src/main.cpp include/bigint.h)

add_executable(tests23 tests/test_bigint.cpp)
target_link_libraries(tests23 PRIVATE lab2task3 GTest::gtest_main)

add_test(NAME Test23 COMMAND tests23)