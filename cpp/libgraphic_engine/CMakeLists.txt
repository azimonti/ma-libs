cmake_minimum_required(VERSION 3.13.4)
project(cpp_graphic_engine)
set (PROJECT_VERSION "1.0"    )
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake_modules")
include( COMPILERCOMMON     )
include( COMPILERCPP        )
include( FindLIBS           )
set(CMAKE_CXX_STANDARD 17   )

if(MSVC)
    generic_libs_find(glew ON       )
    generic_libs_find(glfw ON       )
    generic_libs_find(glm  ON       )
else()
    generic_libs_find(glew OFF      )
    generic_libs_find(glfw OFF      )
    generic_libs_find(glm OFF       )
endif()
generic_libs_find(imgui ON          )
generic_libs_find(implot ON         )
generic_libs_find(imGuIZMO.quat ON  )

include_directories( ${GLM_INCLUDE_DIRS}                 )
include_directories( ${GLEW_INCLUDE_DIRS}                )
include_directories( ${GLEW_INCLUDE_DIRS}/include        )
include_directories( ${GLFW_INCLUDE_DIRS}                )
include_directories( ${GLFW_INCLUDE_DIRS}/include        )
include_directories( ../utils                            )

add_definitions( -DVGIZMO_USES_GLM -DLOGGING             )
add_definitions( -DGLEW_STATIC -DGLM_ENABLE_EXPERIMENTAL )
#if (MSVC)
  #    add_compile_options(/w34389 /w34245 /w34365 /w34388) # Warning about signed/unsigned
  #else()
  #    add_compile_options(-Wall -Wextra -pedantic -Wconversion -Wno-float-conversion -Wno-unused-variable -Wno-unused-parameter -Wno-unused-private-field -Wno-unused-but-set-variable)
  #endif()

set( SRCS_GRAPHIC_ENGINE
    ./src/camera.cpp
    ./src/dglutils.cpp
    ./src/ge_mesh2.cpp
    ./src/ge_mesh3.cpp
    ./src/ge_scenepicking.cpp
    ./src/geomshapes.cpp
    ./src/geomprocessing.cpp
    ./src/light.cpp
    ./src/mesh.cpp
    ./src/scene.cpp
    ./src/shaderprogram.cpp)

link_directories(${LIBS_DIR}          )
link_directories(${GLEW_LIBRARY_PATH} )
link_directories(${GLEW_LIBRARY_PATH2})
link_directories(${GLFW_LIBRARY_PATH} )

add_library( obj${PROJECT_NAME} OBJECT ${SRCS_GRAPHIC_ENGINE}                             )
set_property( TARGET obj${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1              )
if(UNITYBUILD)
    set_property( TARGET obj${PROJECT_NAME} PROPERTY UNITY_BUILD ON                       )
endif()
add_library( ${PROJECT_NAME} STATIC $<TARGET_OBJECTS:obj${PROJECT_NAME}>                  )
set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d"                       )
