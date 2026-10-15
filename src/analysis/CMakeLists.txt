file(GLOB PROBREACH_ANALYSIS_SRC
        ${PROBREACH_SOURCE_DIR}/analysis/formal.cpp
        ${PROBREACH_SOURCE_DIR}/analysis/mc.cpp
        ${PROBREACH_SOURCE_DIR}/util/node_utils.cpp
        ${PROBREACH_SOURCE_DIR}/util/box.cpp
        ${PROBREACH_SOURCE_DIR}/util/box_utils.cpp
        ${PROBREACH_SOURCE_DIR}/util/measurer.cpp
        ${PROBREACH_SOURCE_DIR}/util/sampler.cpp
        ${PROBREACH_SOURCE_DIR}/decision_procedure/decision_procedure.cpp
        ${PROBREACH_SOURCE_DIR}/solver/dreal_solver.cpp
        ${PROBREACH_SOURCE_DIR}/codegen/smt2_generator.cpp)

add_library(probreach_analysis ${PROBREACH_ANALYSIS_SRC})
add_dependencies(probreach_analysis capd)
target_link_libraries(probreach_analysis probreach_frontend ${CAPD_LIBRARIES} ${GSL_LIBRARIES})
