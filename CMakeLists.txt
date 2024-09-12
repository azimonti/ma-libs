cmake_minimum_required(VERSION 3.13.4)
project(ma_libs)
set (PROJECT_VERSION "1.0"    )
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
include( COMPILERCOMMON     )
include( COMPILERCPP        )
include( FindLIBS           )

# used in libnn linker
set(LIBALG_LAPACK_BUILT_FROM_ROOT ON)

add_subdirectory( cpp/libalg_lapack/ )
add_subdirectory( cpp/libnn/         )
