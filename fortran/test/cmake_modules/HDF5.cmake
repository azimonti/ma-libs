if(MSVC)
    generic_libs_find(hdf5 OFF       )
    set(${TEST_PROJECT_NAME}INCS ${HDF5_INCLUDE_DIRS2}/hdf5 )
else()
    generic_libs_find(hdf5 OFF      )
    set(${TEST_PROJECT_NAME}INCS ${HDF5_INCLUDE_DIRS}       )
endif()

set(${TEST_PROJECT_NAME}SRCS ./src/hdf5.test.f90            )
set(${TEST_PROJECT_NAME}LINKLIBSDIRS ${LIBS_DIR} ${HDF5_LIBRARY_PATH}   )

if(MSVC)
    set(${TEST_PROJECT_NAME}LINKLIBS
        debug hdf5_D debug hdf5_fortran_D
        optimized hdf5 optimized hdf5_fortran)
else()
    set(${TEST_PROJECT_NAME}LINKLIBS hdf5 hdf5_fortran       )
endif()

set(TEST_BIN hdf5.test)
