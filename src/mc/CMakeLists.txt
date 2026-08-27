file(GLOB PROBREACH_MC_SRC
        ${PROBREACH_SOURCE_DIR}/mc/mc.cpp
        ${PROBREACH_SOURCE_DIR}/util/rnd.cpp
        ${PROBREACH_SOURCE_DIR}/util/node_utils.cpp
        ${PROBREACH_SOURCE_DIR}/util/box.cpp
        ${PROBREACH_SOURCE_DIR}/util/box_factory.cpp
        ${PROBREACH_SOURCE_DIR}/util/measure.cpp
        ${PROBREACH_SOURCE_DIR}/util/decision_procedure.cpp
        ${PROBREACH_SOURCE_DIR}/solver/dreal_wrapper.cpp
        ${PROBREACH_SOURCE_DIR}/generators/smt2_generator.cpp)

add_library(probreach_mc ${PROBREACH_MC_SRC})
add_dependencies(probreach_mc capd)
target_link_libraries(probreach_mc probreach_frontend ${GSL_LIBRARIES} ${CAPD_LIBRARIES})

add_executable(mc_verifier ${PROBREACH_SOURCE_DIR}/mc/main.cpp)
target_link_libraries(mc_verifier probreach_mc probreach_config)
