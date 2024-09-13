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
    # On Windows, the pybind11 path is different
    set(CMAKE_PREFIX_PATH "$ENV{VIRTUAL_ENV}/Lib/site-packages/pybind11/share/cmake/pybind11")
else()
    # On macOS/Linux, use Python to detect the Python version and set the path
    set(PYTHON_EXECUTABLE "python3")

    # Get the Python version from the current virtual environment
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} -c "import sys; print(f'{sys.version_info.major}.{sys.version_info.minor}')"
        OUTPUT_VARIABLE PYTHON_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    # Set the CMAKE_PREFIX_PATH using the detected Python version
    set(CMAKE_PREFIX_PATH "$ENV{VIRTUAL_ENV}/lib/python${PYTHON_VERSION}/site-packages/pybind11/share/cmake/pybind11")
endif()

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 REQUIRED)
pybind11_add_module(pybind11_add pybind11_add.cpp pybinding_test.cpp binding_test.cpp)

include_directories(../utils)

add_library(static_lib STATIC binding_test.cpp)
add_library(shared_lib SHARED binding_test.cpp)

target_compile_definitions(shared_lib PRIVATE SHARED_LIBS_LINK BUILDING_SHARED_LIBS)

add_executable(main_static main.cpp)
add_executable(main_shared main.cpp)

target_link_libraries(main_static static_lib)
target_link_libraries(main_shared shared_lib)
