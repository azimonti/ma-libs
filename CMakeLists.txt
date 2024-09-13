cmake_minimum_required(VERSION 3.13.4)
project(ma_libs)
set (PROJECT_VERSION "1.0"    )
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

# used in cpp/libnn linker
set(CPP_LIBALG_LAPACK_BUILT_FROM_ROOT ON)

# C++ Libraries
if(CPP_LIBALG_LAPACK OR CPP_LIBNN OR BUILDSUITE)
  add_subdirectory( cpp/libalg_lapack/    )
endif()

if(CPP_LIBNN OR BUILDSUITE)
  add_subdirectory( cpp/libnn/            )
endif()

if(CPP_PYTHON_BINDINGS)
  add_subdirectory( cpp/python_bindings/  )
endif()

# Fortran Libraries
if(F_LIBHDF5UTILS OR BUILDSUITE)
  add_subdirectory( fortran/libhdf5utils/ )
endif()

if(F_LIBALGEBRA OR BUILDSUITE)
  add_subdirectory( fortran/libalgebra/   )
endif()

if(F_LIBALGO OR BUILDSUITE)
  add_subdirectory( fortran/libalgo/      )
endif()

if(F_LIBTEST OR BUILDSUITE)
  add_subdirectory( fortran/libtest/      )
endif()
