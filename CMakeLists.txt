cmake_minimum_required(VERSION 3.10)
# project name
project(ProbReach)

# setting path to additional cmake modules 
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

# setting install/build/test paths, etc.
set(PROBREACH_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(PROBREACH_BINARY_DIR ${CMAKE_BINARY_DIR})
set(PROBREACH_TEST_DIR ${CMAKE_SOURCE_DIR}/test)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# setting version related variables (i.e., MAJOR, MINOR and GIT_SHA1)
set(PROBREACH_VERSION_MAJOR 2)
set(PROBREACH_VERSION_MINOR 0)
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
configure_file(
  "${PROBREACH_SOURCE_DIR}/config/git_sha1.cpp.in"
  "${PROBREACH_BINARY_DIR}/config/git_sha1.cpp" @ONLY)
set_source_files_properties(${PROBREACH_BINARY_DIR}/git_sha1.cpp GENERATED PROPERTIES LANGUAGE CXX)
set(PROBREACH_VERSION ${PROBREACH_VERSION_MAJOR}.${PROBREACH_VERSION_MINOR})
configure_file(
  "${PROBREACH_SOURCE_DIR}/config/version.h.in"
  "${PROBREACH_SOURCE_DIR}/config/version.h")

# setting compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++14 -fopenmp -frounding-math -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-sign-compare -Wno-unused-variable -Wno-return-type -Wno-unused-function")
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# searching for CAPD
find_package(CAPD)
IF(CAPD_FOUND)
  message(STATUS "Found CAPD (headers: ${CAPD_INCLUDE_DIR}; libs: ${CAPD_LIBRARIES})")
  add_custom_target(capd)
ELSE(CAPD_FOUND)
  # setting some variables before building CAPD
  set(CAPD_URL "https://sourceforge.net/projects/capd/files/5.1.2/src/capd-capdDynSys-5.1.2.tar.gz")
  set(CAPD_TMP_PATH "${PROBREACH_BINARY_DIR}/external/tmp-capd")
  set(CAPD_INSTALL_PATH "${PROBREACH_BINARY_DIR}/external/capd" CACHE INTERNAL "")
  set(CAPD_INCLUDE_DIR "${CAPD_INSTALL_PATH}/include")
  set(CAPD_LIBRARIES "${CAPD_INSTALL_PATH}/lib/libcapd.a")
  message(STATUS "CAPD not found at ${CAPD_INSTALL_PATH}")
  message(STATUS ">>> CAPD version from ${CAPD_URL} will be automatically installed to ${CAPD_INSTALL_PATH}")
  message(STATUS ">>> If you want to specify a different location, please specify the following variables:")
  message(STATUS ">>>   CAPD_BUILD_PATH - path where CAPD will be built")
  message(STATUS ">>>   CAPD_INSTALL_PATH - path where CAPD headers and libraries will be installed")
  message(STATUS "")
  message(STATUS ">>> If you have a version of CAPD already installed on your system, please set the following variables:")
  message(STATUS ">>>   CAPD_INCLUDE_DIR - path to CAPD headers")
  message(STATUS ">>>   CAPD_LIBRARIES - path to \"libcapd.a\"")
  message(STATUS "")
  ExternalProject_Add(capd
    PREFIX        ${CAPD_TMP_PATH}
    SOURCE_DIR    "${CAPD_TMP_PATH}/src"
    DOWNLOAD_DIR  "${CAPD_TMP_PATH}/download"
    TMP_DIR       "${CAPD_TMP_PATH}/tmp"
    STAMP_DIR     "${CAPD_TMP_PATH}/capd-stamp"
    INSTALL_DIR   ${CAPD_INSTALL_PATH}
    URL           ${CAPD_URL}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ${CAPD_TMP_PATH}/src/configure
      C=${CMAKE_C_COMPILER}
      XX=${CMAKE_CXX_COMPILER}
      --prefix=${CAPD_INSTALL_PATH}
      --without-gui
	    --with-mpfr=no
      --with-filib=no
    BUILD_COMMAND make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND make install)
  add_custom_command(
    TARGET capd
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CAPD_TMP_PATH})
ENDIF(CAPD_FOUND)
# including CAPD headers
include_directories("${CAPD_INCLUDE_DIR}")

# including all the ProbReach headers
include_directories("${PROBREACH_SOURCE_DIR}")
include_directories("${PROBREACH_SOURCE_DIR}/config")
include_directories("${PROBREACH_SOURCE_DIR}/util")
include_directories("${PROBREACH_SOURCE_DIR}/generators")
include_directories("${PROBREACH_SOURCE_DIR}/solver")
include_directories("${PROBREACH_SOURCE_DIR}/frontend/irep")
include_directories("${PROBREACH_SOURCE_DIR}/frontend/parser")

# searching for GSL
find_package(GSL)
if(GSL_FOUND)
  include_directories("${GSL_INCLUDE_DIRS}")
endif(GSL_FOUND)

# searching for dReal
find_package(dReal)
if(DREAL_FOUND AND DREAL_WORKS)
  message(STATUS "Found dReal: ${DREAL_BIN_PATH}")
  message(STATUS "dReal works; version: ${DREAL_VERSION}")
else(DREAL_FOUND AND DREAL_WORKS)
  if(DREAL_FOUND AND NOT DREAL_WORKS)
    message(STATUS "Found dReal: ${DREAL_BIN_PATH} but it does not work")
  endif(DREAL_FOUND AND NOT DREAL_WORKS)
  # setting some variables
  set(DREAL_URL "https://github.com/dreal/dreal3/releases/download/v3.16.05/dReal-3.16.05-linux.tar.gz")
  set(DREAL_EXTERNAL_TMP_PATH "${PROBREACH_BINARY_DIR}/external/tmp-dreal")
  set(DREAL_EXTERNAL_EXTRACT_PATH "${PROBREACH_BINARY_DIR}/external/dreal")
  set(DREAL_BIN_PATH "${DREAL_EXTERNAL_EXTRACT_PATH}/bin/dReal" CACHE INTERNAL "")
  message(STATUS "dReal not found")
  message(STATUS ">>> dReal version from \"${DREAL_URL}\" will be automatically downloaded to \"${DREAL_BIN_PATH}\"")
  ExternalProject_Add(dreal
    PREFIX        ${DREAL_EXTERNAL_EXTRACT_PATH}
    SOURCE_DIR    ${DREAL_EXTERNAL_EXTRACT_PATH}
    DOWNLOAD_DIR  "${DREAL_EXTERNAL_TMP_PATH}/download"
    TMP_DIR       "${DREAL_EXTERNAL_TMP_PATH}/tmp"
    STAMP_DIR     "${DREAL_EXTERNAL_TMP_PATH}/dreal-stamp"
    BINARY_DIR    "${DREAL_EXTERNAL_TMP_PATH}/dreal-build"
    URL           ${DREAL_URL}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "")
  add_custom_command(
    TARGET dreal
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DREAL_EXTERNAL_TMP_PATH})
endif(DREAL_FOUND AND DREAL_WORKS)

configure_file(
  "${PROBREACH_SOURCE_DIR}/config/env_config.h.in"
  "${PROBREACH_SOURCE_DIR}/config/env_config.h")

# adding CMakeLists.txt files that specify executables
include("${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/src/simulator/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/src/formal/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/src/mc/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/src/config/CMakeLists.txt")

# enabling testing
find_package(GTest)
if(GTest_FOUND)
  include("${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
endif(GTest_FOUND)
