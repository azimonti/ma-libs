set(${TEST_PROJECT_NAME}INCS ../math ../utils)
set(${TEST_PROJECT_NAME}SRCS ./src/lapack_eigen.test.cpp ./src/lapack_lu.test.cpp ./src/lapack_misc.test.cpp
    ./src/lapack_qr.test.cpp ./src/lapack_schur.test.cpp ./src/lapack_svd.test.cpp)
set(${TEST_PROJECT_NAME}LINKLIBSDIRS ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
set(${TEST_PROJECT_NAME}LINKLIBS optimized cpp_alg_lapack debug cpp_alg_lapackd)
add_definitions( -DCOUTEXT -DLOGGING -DUSE_LAPACK -DUSE_BLAS )
set(TEST_BIN lapack.test)
