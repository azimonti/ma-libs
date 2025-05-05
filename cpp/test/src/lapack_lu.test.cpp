#ifndef __clang_analyzer__
/**************************/
/* lapack_schur.test.cpp  */
/*    Version 1.0         */
/*     2023/06/11         */
/**************************/

#include <cassert>
#include <complex>
#include <vector>
#include "algebra/la_blas_mult.h"
#include "algebra/la_lapack_lu.h"
#include "std/cout.h"
#include "doctest.h"
#include "global_static.h"

// clang-format off
# define MREAL1 {4, 4, -3, 3}
# define R1_N 2
# define R1_M 2

# define MCOMPL1 {{4, 0}, {4,0}, {-3, 0}, {3,0}}
# define C1_N 2
# define C1_M 2

# define MCOMPL2F { {5.91f, -5.69f}, {7.09f, 2.72f}, {7.78f, -4.06f}, {-0.79f, -7.21f}, {-3.15f, -4.08f}, {-1.89f, 3.27f}, {4.57f, -2.07f}, {-3.88f, -3.30f}, {-4.89f, 4.20f}, {4.10f, -6.70f}, {3.28f, -3.84f}, {3.84f, 1.19f}}
# define MCOMPL2D { {5.91, -5.69}, {7.09, 2.72}, {7.78, -4.06}, {-0.79, -7.21}, {-3.15, -4.08}, {-1.89, 3.27}, {4.57, -2.07}, {-3.88, -3.30}, {-4.89, 4.20}, {4.10, -6.70}, {3.28, -3.84}, {3.84, 1.19}}
# define C2_N 3
# define C2_M 4

// clang-format on

TEST_SUITE_BEGIN("[LAPACK][LU]");

TEST_CASE("[lapack][LU] float")
{
    la::Matrix<float> A{MREAL1, R1_N, R1_M};
    la::MatrixLU<float> LU{A};
    la::Matrix<float> L_RES{
        {1, 0, -0.75, 1},
        2, 2
    };
    la::Matrix<float> U_RES{
        {4, 4, 0, 6},
        2, 2
    };
    la::Matrix<float> P_RES{
        {1, 0, 0, 1},
        2, 2
    };
    CHECK_NOTHROW(LU.Compute());
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "LU factorization of float matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "L " << LU.L().GetRowsNb() << " x " << LU.L().GetColsNb() << "\n";
        std::cout << LU.L();
        std::cout << "U " << LU.U().GetRowsNb() << " x " << LU.U().GetColsNb() << "\n";
        std::cout << LU.U();
        std::cout << "P" << LU.P().GetRowsNb() << " x " << LU.P().GetColsNb() << "\n";
        std::cout << LU.P();
        std::cout << "A - P * L * U \n";
        std::cout << A - LU.C();
        std::cout << "P * L * U \n";
        std::cout << LU.C();
    }
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(LU.L()[i][j] - L_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(LU.U()[i][j] - U_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(LU.P()[i][j] - P_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(A[i][j] - LU.C()[i][j]) < 1e-5);
}

TEST_CASE("[lapack][LU] double")
{
    la::Matrix<double> A{MREAL1, R1_N, R1_M};
    la::MatrixLU<double> LU{A};
    la::Matrix<double> L_RES{
        {1, 0, -0.75, 1},
        2, 2
    };
    la::Matrix<double> U_RES{
        {4, 4, 0, 6},
        2, 2
    };
    la::Matrix<double> P_RES{
        {1, 0, 0, 1},
        2, 2
    };
    CHECK_NOTHROW(LU.Compute());
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "LU factorization of double matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "L " << LU.L().GetRowsNb() << " x " << LU.L().GetColsNb() << "\n";
        std::cout << LU.L();
        std::cout << "U " << LU.U().GetRowsNb() << " x " << LU.U().GetColsNb() << "\n";
        std::cout << LU.U();
        std::cout << "P" << LU.P().GetRowsNb() << " x " << LU.P().GetColsNb() << "\n";
        std::cout << LU.P();
        std::cout << "A - P * L * U \n";
        std::cout << A - LU.C();
        std::cout << "P * L * U \n";
        std::cout << LU.C();
    }
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(LU.L()[i][j] - L_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(LU.U()[i][j] - U_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(LU.P()[i][j] - P_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(A[i][j] - LU.C()[i][j]) < 1e-5);
}

TEST_CASE("[lapack][LU] complex<float>")
{
    la::Matrix<std::complex<float>> A{MCOMPL1, C1_N, C1_M};
    la::MatrixLU<std::complex<float>> LU{A};
    la::Matrix<std::complex<float>> L_RES{
        {1, 0, -0.75, 1},
        2, 2
    };
    la::Matrix<std::complex<float>> U_RES{
        {4, 4, 0, 6},
        2, 2
    };
    la::Matrix<std::complex<float>> P_RES{
        {1, 0, 0, 1},
        2, 2
    };
    CHECK_NOTHROW(LU.Compute());
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "LU factorization of complex<float>t matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "L " << LU.L().GetRowsNb() << " x " << LU.L().GetColsNb() << "\n";
        std::cout << LU.L();
        std::cout << "U " << LU.U().GetRowsNb() << " x " << LU.U().GetColsNb() << "\n";
        std::cout << LU.U();
        std::cout << "P" << LU.P().GetRowsNb() << " x " << LU.P().GetColsNb() << "\n";
        std::cout << LU.P();
        std::cout << "A - P * L * U \n";
        std::cout << A - LU.C();
        std::cout << "P * L * U \n";
        std::cout << LU.C();
    }
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++)
        {
            REQUIRE(fabs(A[i][j].real() - LU.C()[i][j].real()) < 1e-5);
            REQUIRE(fabs(A[i][j].imag() - LU.C()[i][j].imag()) < 1e-5);
        }
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++)
        {
            REQUIRE(fabs(LU.L()[i][j].real() - L_RES[i][j].real()) < 1e-5);
            REQUIRE(fabs(LU.L()[i][j].imag() - L_RES[i][j].imag()) < 1e-5);
        }
    for (size_t i = 0; i < U_RES.GetRowsNb(); i++)
        for (size_t j = 0; j < U_RES.GetColsNb(); j++)
        {
            REQUIRE(fabs(LU.U()[i][j].real() - U_RES[i][j].real()) < 1e-5);
            REQUIRE(fabs(LU.U()[i][j].imag() - U_RES[i][j].imag()) < 1e-5);
        }
    for (size_t i = 0; i < P_RES.GetRowsNb(); i++)
        for (size_t j = 0; j < P_RES.GetColsNb(); j++)
        {
            REQUIRE(fabs(LU.P()[i][j].real() - P_RES[i][j].real()) < 1e-5);
            REQUIRE(fabs(LU.P()[i][j].imag() - P_RES[i][j].imag()) < 1e-5);
        }
}

TEST_CASE("[lapack][LU] complex<double>")
{
    la::Matrix<std::complex<double>> A{MCOMPL1, C1_N, C1_M};
    la::MatrixLU<std::complex<double>> LU{A};
    la::Matrix<std::complex<double>> L_RES{
        {1, 0, -0.75, 1},
        2, 2
    };
    la::Matrix<std::complex<double>> U_RES{
        {4, 4, 0, 6},
        2, 2
    };
    la::Matrix<std::complex<double>> P_RES{
        {1, 0, 0, 1},
        2, 2
    };
    CHECK_NOTHROW(LU.Compute());
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "LU factorization of complex<double> matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "L " << LU.L().GetRowsNb() << " x " << LU.L().GetColsNb() << "\n";
        std::cout << LU.L();
        std::cout << "U " << LU.U().GetRowsNb() << " x " << LU.U().GetColsNb() << "\n";
        std::cout << LU.U();
        std::cout << "P" << LU.P().GetRowsNb() << " x " << LU.P().GetColsNb() << "\n";
        std::cout << LU.P();
        std::cout << "A - P * L * U \n";
        std::cout << A - LU.C();
        std::cout << "P * L * U \n";
        std::cout << LU.C();
    }
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++)
        {
            REQUIRE(fabs(A[i][j].real() - LU.C()[i][j].real()) < 1e-5);
            REQUIRE(fabs(A[i][j].imag() - LU.C()[i][j].imag()) < 1e-5);
        }
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++)
        {
            REQUIRE(fabs(LU.L()[i][j].real() - L_RES[i][j].real()) < 1e-5);
            REQUIRE(fabs(LU.L()[i][j].imag() - L_RES[i][j].imag()) < 1e-5);
        }
    for (size_t i = 0; i < U_RES.GetRowsNb(); i++)
        for (size_t j = 0; j < U_RES.GetColsNb(); j++)
        {
            REQUIRE(fabs(LU.U()[i][j].real() - U_RES[i][j].real()) < 1e-5);
            REQUIRE(fabs(LU.U()[i][j].imag() - U_RES[i][j].imag()) < 1e-5);
        }
    for (size_t i = 0; i < P_RES.GetRowsNb(); i++)
        for (size_t j = 0; j < P_RES.GetColsNb(); j++)
        {
            REQUIRE(fabs(LU.P()[i][j].real() - P_RES[i][j].real()) < 1e-5);
            REQUIRE(fabs(LU.P()[i][j].imag() - P_RES[i][j].imag()) < 1e-5);
        }
}

TEST_SUITE_END();

#endif
