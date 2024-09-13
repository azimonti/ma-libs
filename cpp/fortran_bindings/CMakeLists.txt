cmake_minimum_required(VERSION 3.13.4)
project(cpp_fortran_bindings)
set (PROJECT_VERSION "1.0")
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake_modules")
include(COMPILERCOMMON)
include(COMPILERCPP)
include(COMPILERFORTRAN)
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 12.4)
endif()

set ( SRCS
    ./src/main.cpp
    )

set ( SRCS_LIB
    ./src/external_function.f90
    )

add_library(${PROJECT_NAME}_static_lib STATIC ${SRCS_LIB})
add_library(${PROJECT_NAME}_shared_lib SHARED ${SRCS_LIB})

add_executable(${PROJECT_NAME}_main_static ${SRCS})
add_executable(${PROJECT_NAME}_main_shared ${SRCS})

target_link_libraries(${PROJECT_NAME}_main_static ${PROJECT_NAME}_static_lib)
target_link_libraries(${PROJECT_NAME}_main_shared ${PROJECT_NAME}_shared_lib)
