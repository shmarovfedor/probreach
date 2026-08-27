file(GLOB SIMULATOR_SRC
        ${PROBREACH_SOURCE_DIR}/util/node_utils.cpp
        ${PROBREACH_SOURCE_DIR}/util/box.cpp
        ${PROBREACH_SOURCE_DIR}/simulator/naive.cpp)

add_library(probreach_simulator ${SIMULATOR_SRC})
add_dependencies(probreach_simulator capd)
target_link_libraries(probreach_simulator probreach_frontend ${CAPD_LIBRARIES})

add_executable(simulator ${PROBREACH_SOURCE_DIR}/simulator/main.cpp)
target_link_libraries(simulator probreach_simulator probreach_config)
