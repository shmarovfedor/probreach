# building and installing PDRH lexer and parser
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# creating a directory for the lexer and parser files
add_custom_target(pdrhparser
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PROBREACH_BINARY_DIR}/frontend/parser
  COMMENT "Creating directory ${PROBREACH_BINARY_DIR}/frontend/parser")
# create custom command for flex/lex (note the outputs)
add_custom_command(
  COMMAND ${FLEX_EXECUTABLE}
  ARGS -o ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhlexer.c
  ${PROBREACH_SOURCE_DIR}/frontend/parser/pdrhlexer.l
  DEPENDS pdrhparser
  DEPENDS ${PROBREACH_SOURCE_DIR}/frontend/parser/pdrhlexer.l
  OUTPUT  ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhlexer.c
  COMMENT "Generating ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhlexer.c")

# create custom command for bison/yacc (note the DEPENDS)
add_custom_command(
  COMMAND ${BISON_EXECUTABLE}
  ARGS -d -y ${PROBREACH_SOURCE_DIR}/frontend/parser/pdrhparser.y
  -o ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhparser.c
  DEPENDS ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhlexer.c
  DEPENDS ${PROBREACH_SOURCE_DIR}/frontend/parser/pdrhparser.y
  OUTPUT  ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhparser.c
  COMMENT "Generating ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhparser.c")

# setting propeties for the generated lexer and parser files
set_source_files_properties(${PROBREACH_BINARY_DIR}/frontend/parser/pdrhparser.c GENERATED PROPERTIES LANGUAGE CXX)
set_source_files_properties(${PROBREACH_BINARY_DIR}/frontend/parser/pdrhlexer.c GENERATED PROPERTIES LANGUAGE CXX)
# including the parser directory
include_directories("${PROBREACH_BINARY_DIR}/frontend/parser")

# building the frontend
file(GLOB PROBREACH_FRONTEND_SRC
        ${PROBREACH_SOURCE_DIR}/frontend/irep/model.cpp
        ${PROBREACH_SOURCE_DIR}/frontend/irep/node.cpp)

set(PROBREACH_FRONTEND_SRC ${PROBREACH_FRONTEND_SRC}      
        ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhparser.c
        ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhlexer.c)

add_library(probreach_frontend ${PROBREACH_FRONTEND_SRC})

if(APPLE)
    target_link_libraries(probreach_frontend -ll)
else(APPLE)
    target_link_libraries(probreach_frontend -lfl)
endif(APPLE)

