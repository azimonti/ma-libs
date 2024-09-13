cmake_minimum_required(VERSION 3.13.4)
project(f_cpp_bindings)
set(PROJECT_VERSION "1.0")
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake_modules")
include( COMPILERCOMMON  )
include( COMPILERCPP     )
include( COMPILERFORTRAN )
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 12.4)
endif()

set ( SRCS
    ./src/main.f90
    )

set ( SRCS_LIB
    ./src/external_function.cpp
    )

include_directories(../../cpp/utils)
add_library(${PROJECT_NAME}_static_lib STATIC ${SRCS_LIB})
add_library(${PROJECT_NAME}_shared_lib SHARED ${SRCS_LIB})
target_compile_definitions(${PROJECT_NAME}_shared_lib PRIVATE SHARED_LIBS_LINK BUILDING_SHARED_LIBS )

add_executable(${PROJECT_NAME}_main_static ${SRCS})
add_executable(${PROJECT_NAME}_main_shared ${SRCS})

target_link_libraries(${PROJECT_NAME}_main_static ${PROJECT_NAME}_static_lib)
target_link_libraries(${PROJECT_NAME}_main_shared ${PROJECT_NAME}_shared_lib)
