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
  ARGS -o ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhlexer.cpp
  ${PROBREACH_SOURCE_DIR}/frontend/parser/pdrhlexer.l
  DEPENDS pdrhparser
  DEPENDS ${PROBREACH_SOURCE_DIR}/frontend/parser/pdrhlexer.l
  OUTPUT  ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhlexer.cpp
  COMMENT "Generating ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhlexer.cpp")

# create custom command for bison/yacc (note the DEPENDS)
add_custom_command(
  COMMAND ${BISON_EXECUTABLE}
  ARGS -d ${PROBREACH_SOURCE_DIR}/frontend/parser/pdrhparser.y
  -o ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhparser.cpp
  DEPENDS ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhlexer.cpp
  DEPENDS ${PROBREACH_SOURCE_DIR}/frontend/parser/pdrhparser.y
  OUTPUT  ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhparser.cpp
  COMMENT "Generating ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhparser.cpp")

# setting propeties for the generated lexer and parser files
set_source_files_properties(${PROBREACH_BINARY_DIR}/frontend/parser/pdrhparser.cpp GENERATED PROPERTIES LANGUAGE CXX)
set_source_files_properties(${PROBREACH_BINARY_DIR}/frontend/parser/pdrhlexer.cpp GENERATED PROPERTIES LANGUAGE CXX)
# including the parser directory
include_directories("${PROBREACH_BINARY_DIR}/frontend/parser")

include_directories("${PROBREACH_SOURCE_DIR}/frontend")
include_directories("${PROBREACH_SOURCE_DIR}/frontend/irep")
include_directories("${PROBREACH_SOURCE_DIR}/frontend/parser")

# building the frontend
file(GLOB PROBREACH_FRONTEND_SRC
        ${PROBREACH_SOURCE_DIR}/frontend/ast/model.cpp
        ${PROBREACH_SOURCE_DIR}/frontend/ast/node.cpp
        ${PROBREACH_SOURCE_DIR}/frontend/ast/new_to_old.cpp
        ${PROBREACH_SOURCE_DIR}/frontend/frontend.cpp)

set(PROBREACH_FRONTEND_SRC ${PROBREACH_FRONTEND_SRC}      
        ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhparser.cpp
        ${PROBREACH_BINARY_DIR}/frontend/parser/pdrhlexer.cpp)

add_library(probreach_frontend ${PROBREACH_FRONTEND_SRC})
