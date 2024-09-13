cmake_minimum_required(VERSION 3.13.4)
project(cpp_nn)
set (PROJECT_VERSION "1.0"    )
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake_modules")
include( COMPILERCOMMON     )
include( COMPILERCPP        )
include( FindLIBS           )
set(CMAKE_CXX_STANDARD 17   )
add_definitions( -DLOGGING -DCOUTEXT -DUSE_BLAS)

if(MSVC)
  # Other optimization are making the results incorrect for SGD
    set(CMAKE_C_FLAGS_RELEASE       " /O1 /Oi /Ob2 /Oy /GS-")
    set(CMAKE_CXX_FLAGS_RELEASE     " /O1 /Oi /Ob2 /Oy /GS-")
    generic_libs_find(hdf5   ON       )
    generic_libs_find(zlib   ON       )
    generic_libs_find(lapack ON       )
    include_directories( ${HDF5_INCLUDE_DIRS}/src )
    include_directories( ${HDF5_INCLUDE_DIRS2}/hdf5 )
    link_directories( ${ZLIB_LIBRARY_PATH}     )
    include_directories( ${LAPACK_INCLUDE_DIRS2}/lapack )
    link_directories( ${LIBS_DIR}/${CMAKE_BUILD_TYPE}   )
    link_directories( ${LAPACK_LIBRARY_PATH2}           )
    link_directories($ENV{INTEL_FORTRAN_LIB_PATH}       )
else()
    generic_libs_find(hdf5 OFF      )
    generic_libs_find(lapack OFF                         )
    include_directories( ${LAPACK_INCLUDE_DIRS2}         )
    link_directories( ${LAPACK_LIBRARY_PATH2}            )
endif()

include_directories( ${HDF5_INCLUDE_DIRS} )
include_directories( ..                   )
include_directories( ../libalg_lapack/src )
include_directories( ../utils             )

link_directories( ${LIBS_DIR}             )
link_directories( ${HDF5_LIBRARY_PATH}    )

set ( SRCS
    ./src/ann_mlp_v1.cpp
    ./src/ann_mlp_ga_v1.cpp
    ./src/ann_mlp_sgd_v1.cpp
    )

set ( SRCS_DEPS_SHARED
    ../utils/hdf5/hdf5_ext.cpp
    ../utils/log/log.cpp
    )

# Conditionally link directories based on whether libalg_lapack was built
if(CPP_LIBALG_LAPACK_BUILT_FROM_ROOT)
    # if compiled from the root directory
    link_directories( ../../build/${CMAKE_BUILD_TYPE} )
else()
    # if compiled separately
    link_directories( ../libalg_lapack/build/${CMAKE_BUILD_TYPE} )
endif()

add_library( obj${PROJECT_NAME} OBJECT ${SRCS}                                            )
set_property( TARGET obj${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1              )
if(UNITYBUILD)
    set_property( TARGET obj${PROJECT_NAME} PROPERTY UNITY_BUILD ON                       )
endif()
add_library( ${PROJECT_NAME}static STATIC $<TARGET_OBJECTS:obj${PROJECT_NAME}>                     )
add_library( ${PROJECT_NAME}       SHARED $<TARGET_OBJECTS:obj${PROJECT_NAME}> ${SRCS_DEPS_SHARED} )

target_compile_definitions( ${PROJECT_NAME} PRIVATE SHARED_LIBS_LINK BUILDING_SHARED_LIBS )
set_target_properties( obj${PROJECT_NAME}      PROPERTIES DEBUG_POSTFIX "d"               )
set_target_properties( ${PROJECT_NAME}static   PROPERTIES DEBUG_POSTFIX "d"               )
set_target_properties( ${PROJECT_NAME}         PROPERTIES DEBUG_POSTFIX "d"               )

if(UNITYBUILD)
    set_property( TARGET ${PROJECT_NAME} PROPERTY UNITY_BUILD ON )
endif()

if(MSVC)
    target_link_libraries( ${PROJECT_NAME}
        debug libhdf5_D debug zlibstaticd debug cpp_alg_lapackd
        optimized libhdf5 optimized zlibstatic optimized cpp_alg_lapack)
else()
    target_link_libraries( ${PROJECT_NAME}
        debug hdf5 debug cpp_alg_lapackd
        optimized hdf5 optimized cpp_alg_lapack)
endif()
