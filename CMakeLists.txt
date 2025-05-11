cmake_minimum_required(VERSION 3.13.4)
project(ma_libs)
set (PROJECT_VERSION "1.0"    )
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

# used in cpp/libnn linker
set(CPP_LIBALG_LAPACK_BUILT_FROM_ROOT ON)

# C++ Libraries
if(CPP_LIBALG_LAPACK OR CPP_LIBNN_BLAS OR CPP_LIBNN_BLAS_HDF5 OR BUILDSUITE)
  add_subdirectory( cpp/libalg_lapack/     )
endif()

if(CPP_LIBNN OR CPP_LIBNN_BLAS OR CPP_LIBNN_BLAS_HDF5 OR BUILDSUITE)
  add_subdirectory( cpp/libnn/             )
endif()

if(CPP_LIBCONFIG_LOADER OR BUILDSUITE)
  add_subdirectory( cpp/libconfig_loader/  )
endif()

if(CPP_LIBGRAPHIC_ENGINE OR BUILDSUITE)
  add_subdirectory( cpp/libgraphic_engine/ )
endif()

if(CPP_PYTHON_BINDINGS)
  add_subdirectory( cpp/python_bindings/   )
endif()

if(CPP_FORTRAN_BINDINGS)
  add_subdirectory( cpp/fortran_bindings/  )
endif()

if(CPP_TEST_SUITE OR CPP_TEST)
  add_subdirectory( cpp/test/              )
endif()

# Fortran Libraries
if(F_LIBHDF5UTILS OR BUILDSUITE)
  add_subdirectory( fortran/libhdf5utils/  )
endif()

if(F_CPP_BINDINGS)
  add_subdirectory( fortran/cpp_bindings/  )
endif()

if(F_LIBALGEBRA OR BUILDSUITE)
  add_subdirectory( fortran/libalgebra/    )
endif()

if(F_LIBALGO OR BUILDSUITE)
  add_subdirectory( fortran/libalgo/       )
endif()

if(F_LIBMATH OR BUILDSUITE)
  add_subdirectory( fortran/libmath/       )
endif()

if(F_LIBTEST OR BUILDSUITE)
  add_subdirectory( fortran/libtest/       )
endif()

if(F_TEST_SUITE OR F_TEST )
  add_subdirectory( fortran/test/          )
endif()
