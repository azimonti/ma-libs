cmake_minimum_required(VERSION 3.13.4)
project(cpp_pythonbindings)
set(PROJECT_VERSION "1.0")
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake_modules")
include(COMPILERCOMMON)
include(COMPILERCPP)
# Ovewrite the c++ version
set(CMAKE_CXX_STANDARD 17)

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
find_package(pybind11 REQUIRED)

set ( SRCS
    ./src/main.cpp
    )

set ( SRCS_LIB
    ./src/binding_test.cpp
    )

set ( SRCS_PYTHON_LIB
    ./src/pybind11_add.cpp
    ./src/pybinding_test.cpp
    )

pybind11_add_module(pybind11_add ${SRCS_PYTHON_LIB} ${SRCS_LIB})

include_directories(../utils)

add_library(${PROJECT_NAME}_static_lib STATIC ${SRCS_LIB})
add_library(${PROJECT_NAME}_shared_lib SHARED ${SRCS_LIB})

target_compile_definitions(${PROJECT_NAME}_shared_lib PRIVATE SHARED_LIBS_LINK BUILDING_SHARED_LIBS)

add_executable(${PROJECT_NAME}_main_static ${SRCS})
add_executable(${PROJECT_NAME}_main_shared ${SRCS})

target_link_libraries(${PROJECT_NAME}_main_static ${PROJECT_NAME}_static_lib)
target_link_libraries(${PROJECT_NAME}_main_shared ${PROJECT_NAME}_shared_lib)
