set(${TEST_PROJECT_NAME}INCS ../math ../utils ../libalg_lapack/src ../)
set(${TEST_PROJECT_NAME}SRCS ./src/matrix.test.cpp)

add_definitions( -DCOUTEXT -DLOGGING )
set(TEST_BIN matrix.test)
