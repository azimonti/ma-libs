project( imgui )

cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17         )
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF      )

include_directories( ${LIBS_ROOT}/imgui            )
include_directories( ${LIBS_ROOT}/imgui/backends   )
if(WIN32)
    include_directories( ${LIBS_ROOT}/glfw/include )
elseif(APPLE)
    include_directories( /usr/local/include        )
else()
    include_directories( /usr/local/include        )
endif()

set( SRCS
    ${LIBS_ROOT}/imgui/imgui.cpp
    ${LIBS_ROOT}/imgui/imgui_widgets.cpp
    ${LIBS_ROOT}/imgui/imgui_draw.cpp
    ${LIBS_ROOT}/imgui/imgui_demo.cpp
    ${LIBS_ROOT}/imgui/imgui_tables.cpp
    ${LIBS_ROOT}/imgui/misc/cpp/imgui_stdlib.cpp
    ${LIBS_ROOT}/imgui/backends/imgui_impl_glfw.cpp
    ${LIBS_ROOT}/imgui/backends/imgui_impl_opengl3.cpp )

add_library( ${PROJECT_NAME} STATIC ${SRCS} )
