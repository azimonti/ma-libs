set(${TEST_PROJECT_NAME}SRCS ./src/hdf5utils.test.f90   )

set(${TEST_PROJECT_NAME}INCS ${CMAKE_SOURCE_DIR}/build/${GIT_BRANCH}/${CMAKE_BUILD_TYPE}/fortran/libhdf5utils/              )
set( ${TEST_PROJECT_NAME}LINKLIBSDIRS ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}   )
set(${TEST_PROJECT_NAME}LINKLIBS debug f_hdf5utilsd optimized f_hdf5utils         )

set(TEST_BIN hdf5utils.test)
