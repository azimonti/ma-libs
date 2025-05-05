set(${TEST_PROJECT_NAME}INCS ../utils)
set(${TEST_PROJECT_NAME}SRCS ./src/logging_release.test.cpp ../utils/log/log.cpp)
set(${TEST_PROJECT_NAME}LINKLIBS)
# remove the logging
remove_definitions( -DLOGGING)
# uncomment for see a 5 seconds delay - preprocessor is optimizing it
# add_definitions( -DLOGGING)
set(TEST_BIN logging_release.test)
