enable_testing()

# setting the output directory for ProbReach
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROBREACH_BINARY_DIR}/test/ast")

# unit test for irep.cpp
add_executable(ast_test ${PROBREACH_TEST_DIR}/ast/ast_test.cpp
        ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(ast_test -lgtest)
add_test(ast_test "${PROBREACH_BINARY_DIR}/test/ast/ast_test")
