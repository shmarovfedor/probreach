find_package(Matlab)
IF(MATLAB_FOUND)
    include_directories("${MATLAB_INCLUDE_DIR}")
    include_directories("${PROBREACH_SOURCE_DIR}/stability")
    add_executable(pid_optimiser ${PROBREACH_SOURCE_DIR}/pid_optimiser/main.cpp ${PROBREACH_SOURCE_DIR}/stability/stability.cpp)
    target_link_libraries(pid_optimiser probreach_mc ${MATLAB_LIBRARIES})
ELSE(MATLAB_FOUND)
    #add_executable(pid_optimiser ${PROBREACH_SOURCE_DIR}/pid_optimiser/main.cpp ${PROBREACH_SOURCE_DIR}/stability/stability.cpp)
    #target_link_libraries(pid_optimiser probreach_mc)
    MESSAGE(WARNING "MATLAB not found, pid_optimiser won't be available.")
ENDIF(MATLAB_FOUND)

