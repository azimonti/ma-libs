cmake_minimum_required(VERSION 3.7)
project( implot                   )
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 13.0)
endif()
set(CMAKE_CXX_STANDARD 17         )
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF      )

include_directories( ${LIBS_ROOT}/imgui          )
include_directories( ${LIBS_ROOT}/imgui/backends )
include_directories( ${LIBS_ROOT}/implot         )

if(WIN32)
    include_directories( ${LIBS_ROOT}/glfw       )
elseif(APPLE)
    include_directories( /usr/local/include      )
else()
    include_directories( /usr/local/include      )
endif()

set( SRCS
    ${LIBS_ROOT}/implot/implot.cpp
    ${LIBS_ROOT}/implot/implot_items.cpp )

set( INCS
    ${LIBS_ROOT}/implot/implot.h
    ${LIBS_ROOT}/implot/implot_internal.h )

add_library( ${PROJECT_NAME} STATIC ${SRCS} ${INCS} )
