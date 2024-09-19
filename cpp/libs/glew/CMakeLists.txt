cmake_minimum_required(VERSION 3.7)
project( glew               )
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../../cmake_modules")
include( COMPILERCOMMON     )
include( COMPILERCPP        )

cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

include_directories( ${LIBS_ROOT}/glew/include )

add_definitions( -DGLEW_STATIC )

set( SRCS ${LIBS_ROOT}/glew/src/glew.c )

add_library( ${PROJECT_NAME} STATIC ${SRCS} )
