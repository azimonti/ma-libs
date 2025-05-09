cmake_minimum_required(VERSION 3.13.4)
project(bind_example)

set (PROJECT_VERSION "1.0"    )
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake_modules")
include( COMPILERCOMMON     )
include( COMPILERCPP        )
include( FindLIBS           )
set(CMAKE_CXX_STANDARD 17   )

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
else()
  message(STATUS "Virtual environment Python not found or OS not handled, letting CMake search.")
endif()

find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

# --- Build the static library ---
add_library(${PROJECT_NAME} STATIC
    ./src/example.cpp
    ./src/example.h
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Add the pybind11 module
# The module name 'example_module' must match PYBIND11_MODULE macro in pybind_example.cpp
pybind11_add_module(example_module SHARED
    ./src/pybind_example.cpp
    ./src/example_py.cpp
)

# Link the Python module against the static library
target_link_libraries(example_module PRIVATE ${PROJECT_NAME})

# add the C++ binary
add_executable(${PROJECT_NAME}_main ./src/main.cpp)
target_link_libraries(${PROJECT_NAME}_main PRIVATE bind_example)

if(UNITYBUILD)
  set_property( TARGET ${PROJECT_NAME}      PROPERTY UNITY_BUILD ON                     )
  set_property( TARGET ${PROJECT_NAME}_main PROPERTY UNITY_BUILD ON                     )
  set_property( TARGET example_module       PROPERTY UNITY_BUILD ON                     )
endif()
