file(GLOB PROBREACH_CONFIG_SRC
        ${PROBREACH_SOURCE_DIR}/config/pdrh_config.cpp
        ${PROBREACH_BINARY_DIR}/config/git_sha1.cpp)

add_library(probreach_config ${PROBREACH_CONFIG_SRC})
