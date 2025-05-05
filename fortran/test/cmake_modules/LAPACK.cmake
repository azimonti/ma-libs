set(${TEST_PROJECT_NAME}SRCS ./src/lapack.test.f90   )

set(${TEST_PROJECT_NAME}INCS ${CMAKE_SOURCE_DIR}/build/${GIT_BRANCH}/${CMAKE_BUILD_TYPE}/fortran/libmath/                )

if(MSVC)
    include( FindLIBS                 )
    generic_libs_find(lapack ON       )
    set(${TEST_PROJECT_NAME}LINKLIBSDIRS ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} ${LIBS_DIR}/${CMAKE_BUILD_TYPE}   )
elseif(APPLE)
    set(${TEST_PROJECT_NAME}LINKLIBSDIRS ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE} /usr/local/opt/lapack/lib         )
endif()

set(${TEST_PROJECT_NAME}LINKLIBS debug f_mathutilsd optimized f_mathutils lapack blas )

set(TEST_BIN lapack.test)
