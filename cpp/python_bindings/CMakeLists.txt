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
