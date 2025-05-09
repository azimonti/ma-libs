cmake_minimum_required(VERSION 3.13.4)
project(config_loader)
set (PROJECT_VERSION "1.0"    )
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake_modules")
include( COMPILERCOMMON     )
include( COMPILERCPP        )

set ( SRCS
  ./src/config_loader.cpp
)

add_library( obj${PROJECT_NAME} OBJECT ${SRCS}                                            )
set_property( TARGET obj${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1              )
if(UNITYBUILD)
  set_property( TARGET obj${PROJECT_NAME} PROPERTY UNITY_BUILD ON                         )
endif()
add_library( ${PROJECT_NAME}static STATIC $<TARGET_OBJECTS:obj${PROJECT_NAME}>            )
add_library( ${PROJECT_NAME}       SHARED $<TARGET_OBJECTS:obj${PROJECT_NAME}>            )

set_target_properties( obj${PROJECT_NAME}      PROPERTIES DEBUG_POSTFIX "d"               )
set_target_properties( ${PROJECT_NAME}static   PROPERTIES DEBUG_POSTFIX "d"               )
set_target_properties( ${PROJECT_NAME}         PROPERTIES DEBUG_POSTFIX "d"               )

if(UNITYBUILD)
  set_property( TARGET obj${PROJECT_NAME}     PROPERTY UNITY_BUILD ON                     )
  set_property( TARGET ${PROJECT_NAME}static  PROPERTY UNITY_BUILD ON                     )
  set_property( TARGET ${PROJECT_NAME}        PROPERTY UNITY_BUILD ON                     )
endif()
