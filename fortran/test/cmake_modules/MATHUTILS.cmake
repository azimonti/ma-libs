set(${TEST_PROJECT_NAME}SRCS ./src/mathutils.test.f90   )
set(${TEST_PROJECT_NAME}INCS ${CMAKE_SOURCE_DIR}/build/${GIT_BRANCH}/${CMAKE_BUILD_TYPE}/fortran/libmath/  )
set( ${TEST_PROJECT_NAME}LINKLIBSDIRS ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}   )
set(${TEST_PROJECT_NAME}LINKLIBS debug f_mathutilsd optimized f_mathutils    )

set(TEST_BIN mathutils.test)
