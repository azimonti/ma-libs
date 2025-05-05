set(${TEST_PROJECT_NAME}INCS ../utils)
set(${TEST_PROJECT_NAME}SRCS ./src/logging.test.cpp ../utils/log/log.cpp)
set(${TEST_PROJECT_NAME}LINKLIBS)
# Define the logging
add_definitions( -DLOGGING )
set(TEST_BIN logging.test)
