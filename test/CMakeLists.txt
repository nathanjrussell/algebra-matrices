add_executable(algebra_matrices_test matrix_test.cpp)
target_link_libraries(algebra_matrices_test PRIVATE algebra::matrices algebra::finite_fields GTest::gtest_main)
gtest_discover_tests(algebra_matrices_test TEST_PREFIX "algebra_matrices.")
