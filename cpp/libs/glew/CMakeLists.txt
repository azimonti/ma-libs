cmake_minimum_required(VERSION 3.7)
project( glew               )
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 13.0)
endif()
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

include_directories( ${LIBS_ROOT}/glew/include )

add_definitions( -DGLEW_STATIC )

set( SRCS ${LIBS_ROOT}/glew/src/glew.c )

add_library( ${PROJECT_NAME} STATIC ${SRCS} )
