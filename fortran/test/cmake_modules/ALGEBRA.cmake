set(${TEST_PROJECT_NAME}SRCS ./src/algebra.test.f90   )

set(${TEST_PROJECT_NAME}INCS ../libalgebra/build/${GIT_BRANCH}                  )
set( ${TEST_PROJECT_NAME}LINKLIBSDIRS ../libalgebra/build/${CMAKE_BUILD_TYPE}   )
set(${TEST_PROJECT_NAME}LINKLIBS debug algebra_fd optimized algebra_f           )

set(TEST_BIN algebra.test)
