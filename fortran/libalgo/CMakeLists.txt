cmake_minimum_required(VERSION 3.13.4)
project( f_algo )
set(PROJECT_VERSION "1.0")
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake_modules")
include( COMPILERCOMMON  )
include( COMPILERFORTRAN )

link_directories(${HDF5_LIBRARY_PATH}        )

if(MSVC)
    include_directories( $ENV{IFORT_COMPILER22}/compiler/include        )
    link_directories   ( $ENV{IFORT_COMPILER22}/compiler/lib/intel64    )
endif()

set( SRCS algo_utils.f90 )

add_library( obj${PROJECT_NAME} OBJECT ${SRCS}                                            )
set_property( TARGET obj${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1              )
add_library( ${PROJECT_NAME}static STATIC $<TARGET_OBJECTS:obj${PROJECT_NAME}>            )

set_target_properties( obj${PROJECT_NAME}      PROPERTIES DEBUG_POSTFIX "d"               )
set_target_properties( ${PROJECT_NAME}static   PROPERTIES DEBUG_POSTFIX "d"               )
add_library( ${PROJECT_NAME}       SHARED $<TARGET_OBJECTS:obj${PROJECT_NAME}>            )
set_target_properties( ${PROJECT_NAME}         PROPERTIES DEBUG_POSTFIX "d"               )
