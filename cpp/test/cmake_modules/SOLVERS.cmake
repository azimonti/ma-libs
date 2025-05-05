set(${TEST_PROJECT_NAME}INCS ../math ../utils)
set(${TEST_PROJECT_NAME}SRCS ./src/solvers.test.cpp)
add_definitions( -DCOUTEXT )
set(TEST_BIN solvers.test)
