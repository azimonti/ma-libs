cmake_minimum_required(VERSION 3.13.4)
project(cpp_nn2)
set (PROJECT_VERSION "1.0"    )
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake_modules")
include( COMPILERCOMMON     )
include( COMPILERCPP        )
include( FindLIBS           )
set(CMAKE_CXX_STANDARD 17   )
add_definitions( -DLOGGING -DCOUTEXT )

include_directories( ..                   )
include_directories( ../utils             )

set ( SRCS
  ./src/ann_mlp_v1.cpp
  ./src/ann_mlp_ga_v1.cpp
  ./src/ann_mlp_sgd_v1.cpp
)

set ( SRCS_DEPS_LOG
  ../utils/log/log.cpp
)

add_library( obj${PROJECT_NAME} OBJECT ${SRCS}                                            )
set_property( TARGET obj${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1              )
if(UNITYBUILD)
  set_property( TARGET obj${PROJECT_NAME} PROPERTY UNITY_BUILD ON                         )
endif()
add_library( ${PROJECT_NAME}static STATIC $<TARGET_OBJECTS:obj${PROJECT_NAME}>            )

set_target_properties( obj${PROJECT_NAME}      PROPERTIES DEBUG_POSTFIX "d"               )
set_target_properties( ${PROJECT_NAME}static   PROPERTIES DEBUG_POSTFIX "d"               )

if(UNITYBUILD)
  set_property( TARGET obj${PROJECT_NAME}     PROPERTY UNITY_BUILD ON                     )
  set_property( TARGET ${PROJECT_NAME}static  PROPERTY UNITY_BUILD ON                     )
endif()

if(PYTHON_LIBNN)
  project(cpp_nn_py2)
  if(WIN32)
    set(EXPECTED_PYTHON_EXE "$ENV{VIRTUAL_ENV}/Scripts/python.exe")
    # On Windows, the pybind11 path is different
    list(APPEND CMAKE_PREFIX_PATH "$ENV{VIRTUAL_ENV}/Lib/site-packages/pybind11/share/cmake/pybind11")
  elseif(APPLE OR UNIX) # Covers macOS and Linux/other Unix
    set(EXPECTED_PYTHON_EXE "$ENV{VIRTUAL_ENV}/bin/python3") # Usually python3 in venvs
    # Get the Python version from the current virtual environment
    execute_process(
      COMMAND ${EXPECTED_PYTHON_EXE} -c "import sys; print(f'{sys.version_info.major}.{sys.version_info.minor}')"
      OUTPUT_VARIABLE PYTHON_VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET
    )
    # Set the CMAKE_PREFIX_PATH using the detected Python version
    if(PYTHON_VERSION)
      list(APPEND CMAKE_PREFIX_PATH "$ENV{VIRTUAL_ENV}/lib/python${PYTHON_VERSION}/site-packages/pybind11/share/cmake/pybind11")
    else()
      message(WARNING "Could not determine Python version for pybind11 path.")
    endif()
  endif()

  if(DEFINED EXPECTED_PYTHON_EXE AND EXISTS "${EXPECTED_PYTHON_EXE}")
    # Hint for find_package
    set(Python_EXECUTABLE "${EXPECTED_PYTHON_EXE}" CACHE FILEPATH "Path to Python executable from venv")
    message(STATUS "Hinting Python_EXECUTABLE to venv: ${Python_EXECUTABLE}")
  else()
    message(STATUS "Virtual environment Python not found or OS not handled, letting CMake search.")
  endif()

  find_package(Python COMPONENTS Interpreter Development REQUIRED)

  message(STATUS "Using Python: ${Python_EXECUTABLE} version ${Python_VERSION}")
  find_package(pybind11 REQUIRED)

  # RPATH settings for macOS/Linux to find libs in the same build directory
  if(NOT WIN32)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    if(APPLE)
      set(CMAKE_INSTALL_RPATH "@loader_path")
    else() # Linux
      set(CMAKE_INSTALL_RPATH "$ORIGIN")
    endif()
  endif()

  set ( SRCS_PYTHON_LIB
    ./src/ann_mlp_ga_py_interface.cpp
  )

  pybind11_add_module(${PROJECT_NAME} ${SRCS_PYTHON_LIB})

  target_link_libraries( ${PROJECT_NAME} PRIVATE cpp_nn2static)
endif()

project(cpp_nn)

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
include_directories( ../libalg_lapack/src )

link_directories( ${LIBS_DIR}             )
link_directories( ${HDF5_LIBRARY_PATH}    )


set ( SRCS_DEPS_HDF5
    ../utils/hdf5/hdf5_ext.cpp
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
add_library( ${PROJECT_NAME}static STATIC $<TARGET_OBJECTS:obj${PROJECT_NAME}>            )
add_library( ${PROJECT_NAME}       SHARED $<TARGET_OBJECTS:obj${PROJECT_NAME}> ${SRCS_DEPS_LOG} ${SRCS_DEPS_HDF5} )

target_compile_definitions( ${PROJECT_NAME} PRIVATE SHARED_LIBS_LINK BUILDING_SHARED_LIBS )

# using BLAS and HDF5 only for these targets
target_compile_definitions( obj${PROJECT_NAME}    PRIVATE USE_BLAS USE_HDF5               )
target_compile_definitions( ${PROJECT_NAME}       PRIVATE USE_BLAS USE_HDF5               )
target_compile_definitions( ${PROJECT_NAME}static PRIVATE USE_BLAS USE_HDF5               )
set_target_properties( obj${PROJECT_NAME}      PROPERTIES DEBUG_POSTFIX "d"               )
set_target_properties( ${PROJECT_NAME}static   PROPERTIES DEBUG_POSTFIX "d"               )
set_target_properties( ${PROJECT_NAME}         PROPERTIES DEBUG_POSTFIX "d"               )

if(UNITYBUILD)
    set_property( TARGET obj${PROJECT_NAME}     PROPERTY UNITY_BUILD ON                   )
    set_property( TARGET ${PROJECT_NAME}static  PROPERTY UNITY_BUILD ON                   )
    set_property( TARGET ${PROJECT_NAME}        PROPERTY UNITY_BUILD ON                   )
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

if(PYTHON_LIBNN)
    project(cpp_nn_py)
    find_package(pybind11 REQUIRED)

    # RPATH settings for macOS/Linux to find libs in the same build directory
    if(NOT WIN32)
        set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
        if(APPLE)
            set(CMAKE_INSTALL_RPATH "@loader_path")
        else() # Linux
            set(CMAKE_INSTALL_RPATH "$ORIGIN")
        endif()
        # set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE) # Optional
    endif()

    set ( SRCS_PYTHON_LIB
        ./src/ann_mlp_ga_py_interface.cpp
        )

    pybind11_add_module(${PROJECT_NAME} ${SRCS_PYTHON_LIB})

    target_compile_definitions( ${PROJECT_NAME}       PRIVATE USE_BLAS USE_HDF5               )
    target_link_libraries( ${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:cpp_nnd> # Link debug nn lib in Debug config
        $<$<NOT:$<CONFIG:Debug>>:cpp_nn> # Link release nn lib in other configs (Release, etc.)
        )
endif()
