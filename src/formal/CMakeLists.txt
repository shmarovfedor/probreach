file(GLOB PROBREACH_FORMAL_SRC
        ${PROBREACH_SOURCE_DIR}/formal/formal.cpp
        ${PROBREACH_SOURCE_DIR}/util/node_utils.cpp
        ${PROBREACH_SOURCE_DIR}/util/box.cpp
        ${PROBREACH_SOURCE_DIR}/util/box_factory.cpp
        ${PROBREACH_SOURCE_DIR}/util/measure.cpp
        ${PROBREACH_SOURCE_DIR}/util/decision_procedure.cpp
        ${PROBREACH_SOURCE_DIR}/solver/dreal_wrapper.cpp
        ${PROBREACH_SOURCE_DIR}/generators/smt2_generator.cpp)

add_library(probreach_formal ${PROBREACH_FORMAL_SRC})
add_dependencies(probreach_formal capd)
target_link_libraries(probreach_formal probreach_frontend ${CAPD_LIBRARIES})

add_executable(formal_verifier ${PROBREACH_SOURCE_DIR}/formal/main.cpp)
target_link_libraries(formal_verifier probreach_formal probreach_config)
