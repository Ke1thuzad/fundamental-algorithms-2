add_library(lab3task1 src/main.cpp include/bst.h)

add_executable(tests31 tests/test_bst.cpp)
target_link_libraries(tests31 PRIVATE lab3task1 GTest::gtest_main)

add_test(NAME Test31 COMMAND tests31)