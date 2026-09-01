enable_testing()

# setting the output directory for ProbReach
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROBREACH_BINARY_DIR}/test/irep")

# unit test for irep.cpp
add_executable(irep_test ${PROBREACH_TEST_DIR}/irep/irep_test.cpp
        ${PROBREACH_TEST_DIR}/main_test.cpp
        ${PROBREACH_SOURCE_DIR}/frontend/irep/irep.cpp)
target_link_libraries(irep_test -lgtest)
add_test(irep_test "${PROBREACH_BINARY_DIR}/test/irep/irep_test")
