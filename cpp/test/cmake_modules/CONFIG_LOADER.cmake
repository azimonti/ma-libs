set(${TEST_PROJECT_NAME}INCS ../utils ../libconfig_loader/src)
set(${TEST_PROJECT_NAME}SRCS ./src/config_loader.test.cpp)
set(${TEST_PROJECT_NAME}LINKLIBSDIRS ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
set(${TEST_PROJECT_NAME}LINKLIBS optimized config_loader debug config_loaderd)

set(TEST_BIN config_loader.test  )
