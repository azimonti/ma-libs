set(${TEST_PROJECT_NAME}INCS ../utils)
set(${TEST_PROJECT_NAME}SRCS ./src/cout.test.cpp)
set(${TEST_PROJECT_NAME}LINKLIBS)
# Define COUT and COUTM
add_definitions( -DCOUTEXT )
set(TEST_BIN cout.test  )
