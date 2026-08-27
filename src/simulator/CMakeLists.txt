file(GLOB SIMULATOR_SRC
        ${PROBREACH_SOURCE_DIR}/util/node.cpp
        ${PROBREACH_SOURCE_DIR}/util/model.cpp
        ${PROBREACH_SOURCE_DIR}/util/node_utils.cpp
        ${PROBREACH_SOURCE_DIR}/util/box.cpp
        ${PROBREACH_SOURCE_DIR}/simulator/naive.cpp)

set(SIMULATOR_SRC ${SIMULATOR_SRC} ${PROBREACH_BINARY_DIR}/parser/pdrhparser.c
        ${PROBREACH_BINARY_DIR}/parser/pdrhlexer.c
        ${PROBREACH_BINARY_DIR}/git_sha1.cpp)

add_library(probreach_simulator ${SIMULATOR_SRC})
add_dependencies(probreach_simulator capd)

if(APPLE)
    target_link_libraries(probreach_simulator -ll)
else(APPLE)
    target_link_libraries(probreach_simulator -lfl)
endif(APPLE)

target_link_libraries(probreach_simulator ${CAPD_LIBRARIES})

add_executable(simulator ${PROBREACH_SOURCE_DIR}/simulator/main.cpp)
target_link_libraries(simulator probreach_simulator)
