set(${TEST_PROJECT_NAME}SRCS ./src/algorithm.test.f90   )

set(${TEST_PROJECT_NAME}INCS ../libalgo/build/${GIT_BRANCH}                )
set( ${TEST_PROJECT_NAME}LINKLIBSDIRS ../libalgo/build/${CMAKE_BUILD_TYPE} )
set(${TEST_PROJECT_NAME}LINKLIBS debug algo_fd optimized algo_f            )

set(TEST_BIN algorithm.test)
