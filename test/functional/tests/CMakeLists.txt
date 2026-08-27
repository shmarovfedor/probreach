enable_testing()

include_directories("${PROBREACH_SOURCE_DIR}/")
include_directories("${PROBREACH_SOURCE_DIR}/util")
include_directories("${PROBREACH_SOURCE_DIR}/frontend/parser")
include_directories("${PROBREACH_SOURCE_DIR}/frontend/irep")
include_directories("${PROBREACH_SOURCE_DIR}/formal")
include_directories("${PROBREACH_SOURCE_DIR}/mc")
include_directories("${PROBREACH_SOURCE_DIR}/config")

# setting the output directory for ProbReach
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROBREACH_BINARY_DIR}/test/functional/tests")

# Formal tests start here
add_executable(formal_good_test_1 
  ${PROBREACH_TEST_DIR}/functional/tests/formal_good_test_1.cpp
  ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(formal_good_test_1 probreach_formal probreach_config -lgtest)
add_test(formal_good_test_1 "${PROBREACH_BINARY_DIR}/test/functional/tests/formal_good_test_1")

add_executable(formal_good_test_2
  ${PROBREACH_TEST_DIR}/functional/tests/formal_good_test_2.cpp
  ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(formal_good_test_2 probreach_formal probreach_config -lgtest)
add_test(formal_good_test_2 "${PROBREACH_BINARY_DIR}/test/functional/tests/formal_good_test_2")

add_executable(formal_good_test_3 
  ${PROBREACH_TEST_DIR}/functional/tests/formal_good_test_3.cpp
  ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(formal_good_test_3 probreach_formal probreach_config -lgtest)
add_test(formal_good_test_3 "${PROBREACH_BINARY_DIR}/test/functional/tests/formal_good_test_3")

add_executable(formal_good_test_4 
  ${PROBREACH_TEST_DIR}/functional/tests/formal_good_test_4.cpp
  ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(formal_good_test_4 probreach_formal probreach_config -lgtest)
add_test(formal_good_test_4 "${PROBREACH_BINARY_DIR}/test/functional/tests/formal_good_test_4")

add_executable(formal_bad_test_1 
  ${PROBREACH_TEST_DIR}/functional/tests/formal_bad_test_1.cpp
  ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(formal_bad_test_1 probreach_formal probreach_config -lgtest)
add_test(formal_bad_test_1 "${PROBREACH_BINARY_DIR}/test/functional/tests/formal_bad_test_1")

add_executable(formal_bad_test_2 
  ${PROBREACH_TEST_DIR}/functional/tests/formal_bad_test_2.cpp
  ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(formal_bad_test_2 probreach_formal probreach_config -lgtest)
add_test(formal_bad_test_2 "${PROBREACH_BINARY_DIR}/test/functional/tests/formal_bad_test_2")

add_executable(formal_bad_test_3
  ${PROBREACH_TEST_DIR}/functional/tests/formal_bad_test_3.cpp
  ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(formal_bad_test_3 probreach_formal probreach_config -lgtest)
add_test(formal_bad_test_3 "${PROBREACH_BINARY_DIR}/test/functional/tests/formal_bad_test_3")

add_executable(formal_bad_test_4
  ${PROBREACH_TEST_DIR}/functional/tests/formal_bad_test_4.cpp
  ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(formal_bad_test_4 probreach_formal probreach_config -lgtest)
add_test(formal_bad_test_4 "${PROBREACH_BINARY_DIR}/test/functional/tests/formal_bad_test_4")

add_executable(formal_car_stopping_nonlinear_test_1
  ${PROBREACH_TEST_DIR}/functional/tests/formal_car_stopping_nonlinear_test_1.cpp
  ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(formal_car_stopping_nonlinear_test_1 probreach_formal probreach_config -lgtest)
add_test(formal_car_stopping_nonlinear_test_1 "${PROBREACH_BINARY_DIR}/test/functional/tests/formal_car_stopping_nonlinear_test_1")

# Monte Carlo tests start here
add_executable(mc_good_test_1 
  ${PROBREACH_TEST_DIR}/functional/tests/mc_good_test_1.cpp
  ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(mc_good_test_1 probreach_mc probreach_config -lgtest)
add_test(mc_good_test_1 "${PROBREACH_BINARY_DIR}/test/functional/tests/mc_good_test_1")

add_executable(mc_good_test_4 
  ${PROBREACH_TEST_DIR}/functional/tests/mc_good_test_4.cpp
  ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(mc_good_test_4 probreach_mc probreach_config -lgtest)
add_test(mc_good_test_4 "${PROBREACH_BINARY_DIR}/test/functional/tests/mc_good_test_4")

add_executable(mc_bad_test_4 
  ${PROBREACH_TEST_DIR}/functional/tests/mc_bad_test_4.cpp
  ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(mc_bad_test_4 probreach_mc probreach_config -lgtest)
add_test(mc_bad_test_4 "${PROBREACH_BINARY_DIR}/test/functional/tests/mc_bad_test_4")

add_executable(mc_car_stopping_nonlinear_test_1
  ${PROBREACH_TEST_DIR}/functional/tests/mc_car_stopping_nonlinear_test_1.cpp
  ${PROBREACH_TEST_DIR}/main_test.cpp)
target_link_libraries(mc_car_stopping_nonlinear_test_1 probreach_mc probreach_config -lgtest)
add_test(mc_car_stopping_nonlinear_test_1 "${PROBREACH_BINARY_DIR}/test/functional/tests/mc_car_stopping_nonlinear_test_1")
