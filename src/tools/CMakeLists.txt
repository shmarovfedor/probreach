add_executable(formal_verifier ${PROBREACH_SOURCE_DIR}/tools/formal_verifier.cpp)
target_link_libraries(formal_verifier probreach_analysis probreach_config)

add_executable(mc_verifier ${PROBREACH_SOURCE_DIR}/tools/mc_verifier.cpp)
target_link_libraries(mc_verifier probreach_analysis probreach_config)
