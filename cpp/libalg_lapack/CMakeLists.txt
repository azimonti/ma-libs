cmake_minimum_required(VERSION 3.13.4)
project(cpp_alg_lapack)
set (PROJECT_VERSION "1.0"    )
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake_modules")
include( COMPILERCOMMON     )
include( COMPILERCPP        )
include( FindLIBS           )
set(CMAKE_CXX_STANDARD 17   )

if(MSVC)
    generic_libs_find(lapack ON       )
     include_directories( ${LAPACK_INCLUDE_DIRS2}/lapack )
     link_directories( ${LIBS_DIR}/${CMAKE_BUILD_TYPE}   )
     link_directories( ${LAPACK_LIBRARY_PATH2}           )
     link_directories($ENV{INTEL_FORTRAN_LIB_PATH}       )
elseif(APPLE)
    generic_libs_find(lapack OFF                         )
    include_directories( ${LAPACK_INCLUDE_DIRS2}         )
    link_directories( ${LAPACK_LIBRARY_PATH2}            )
endif()

add_definitions( -DLOGGING -DCOUTEXT -DUSE_LAPACK -DUSE_BLAS)

include_directories( ../         )
include_directories( ../utils    )

set ( SRCS
    ./src/la_blas_mult.cpp
    ./src/la_lapack_eigen.cpp
    ./src/la_lapack_lu.cpp
    ./src/la_lapack_misc.cpp
    ./src/la_lapack_qr.cpp
    ./src/la_lapack_schur.cpp
    ./src/la_lapack_svd.cpp
    )

set ( SRCS_DEPS_SHARED
    ../utils/log/log.cpp
    )

link_directories(${LIBS_DIR}             )
add_library( obj${PROJECT_NAME} OBJECT ${SRCS}                                            )
set_property( TARGET obj${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1              )
if(UNITYBUILD)
    set_property( TARGET obj${PROJECT_NAME} PROPERTY UNITY_BUILD ON                       )
endif()
add_library( ${PROJECT_NAME}static STATIC $<TARGET_OBJECTS:obj${PROJECT_NAME}>                     )
add_library( ${PROJECT_NAME}       SHARED $<TARGET_OBJECTS:obj${PROJECT_NAME}> ${SRCS_DEPS_SHARED} )

target_compile_definitions( ${PROJECT_NAME} PRIVATE SHARED_LIBS_LINK BUILDING_SHARED_LIBS )
set_target_properties( ${PROJECT_NAME}static   PROPERTIES DEBUG_POSTFIX "d"               )
set_target_properties( ${PROJECT_NAME}         PROPERTIES DEBUG_POSTFIX "d"               )

if (MSVC)
    target_link_libraries(${PROJECT_NAME} lapack blas           )
    target_link_libraries(${PROJECT_NAME}static lapack blas     )
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} lapack blas           )
    target_link_libraries(${PROJECT_NAME}static lapack blas     )
else()
    target_link_libraries(${PROJECT_NAME} lapack blas           )
    target_link_libraries(${PROJECT_NAME}static lapack blas     )
endif()
