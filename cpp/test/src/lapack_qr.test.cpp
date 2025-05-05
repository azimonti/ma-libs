#ifndef __clang_analyzer__
/**************************/
/*   lapack_qr.test.cpp   */
/*    Version 1.0         */
/*     2023/05/21         */
/**************************/

#include <cassert>
#include <complex>
#include <vector>
#include "algebra/la_blas_mult.h"
#include "algebra/la_lapack_qr.h"
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
TEST_SUITE_BEGIN("[LAPACK][QR]");

TEST_CASE("[lapack][QR] float")
{
    la::Matrix<float> A{MREAL1, R1_N, R1_M};
    int flags = 0;
    la::MatrixQR<float> QR{A};
    la::Matrix<float> Q_RES{
        {-0.8f, 0.6f, 0.6f, 0.8f},
        R1_N, R1_M
    };
    la::Matrix<float> R_RES{
        {-5.f, -1.4f, 0.f, 4.8f},
        R1_N, R1_M
    };
    CHECK_NOTHROW(QR.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "QR factorization of float matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "Q " << QR.Q().GetRowsNb() << " x " << QR.Q().GetColsNb() << "\n";
        std::cout << QR.Q();
        std::cout << "R " << QR.R().GetRowsNb() << " x " << QR.R().GetColsNb() << "\n";
        std::cout << QR.R();
        std::cout << "A - Q * R \n";
        std::cout << A - QR.C();
        std::cout << "Q * R \n";
        std::cout << QR.C();
    }
    for (size_t i = 0; i < QR.Q().GetRowsNb(); i++)
        for (size_t j = 0; j < QR.Q().GetColsNb(); j++) REQUIRE(fabs(QR.Q()[i][j] - Q_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < QR.R().GetRowsNb(); i++)
        for (size_t j = 0; j < QR.R().GetColsNb(); j++) REQUIRE(fabs(QR.R()[i][j] - R_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(A[i][j] - QR.C()[i][j]) < 1e-5);
}

TEST_CASE("[lapack][QR] double")
{
    la::Matrix<double> A{MREAL1, R1_N, R1_M};
    int flags = 0;
    la::MatrixQR<double> QR{A};
    la::Matrix<double> Q_RES{
        {-0.8, 0.6, 0.6, 0.8},
        R1_N, R1_M
    };
    la::Matrix<double> R_RES{
        {-5, -1.4, 0, 4.8},
        R1_N, R1_M
    };
    CHECK_NOTHROW(QR.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "QR factorization of double matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "Q " << QR.Q().GetRowsNb() << " x " << QR.Q().GetColsNb() << "\n";
        std::cout << QR.Q();
        std::cout << "R " << QR.R().GetRowsNb() << " x " << QR.R().GetColsNb() << "\n";
        std::cout << QR.R();
        std::cout << "A - Q * R \n";
        std::cout << A - QR.C();
        std::cout << "Q * R \n";
        std::cout << QR.C();
    }
    for (size_t i = 0; i < QR.Q().GetRowsNb(); i++)
        for (size_t j = 0; j < QR.Q().GetColsNb(); j++) REQUIRE(fabs(QR.Q()[i][j] - Q_RES[i][j]) < 1e-9);
    for (size_t i = 0; i < QR.R().GetRowsNb(); i++)
        for (size_t j = 0; j < QR.R().GetColsNb(); j++) REQUIRE(fabs(QR.R()[i][j] - R_RES[i][j]) < 1e-9);
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(A[i][j] - QR.C()[i][j]) < 1e-9);
}

TEST_CASE("[lapack][QR] complex<float>")
{
    la::Matrix<std::complex<float>> A{MCOMPL1, C1_N, C1_M}, Q{C1_N, C1_N};
    int flags = 0;
    la::MatrixQR<std::complex<float>> QR{A};
    la::Matrix<std::complex<float>> Q_RES{{{{-0.8f, 0.f}, {0.6f, 0.f}, {0.6f, 0.f}, {0.8f, 0.f}}}, C1_N, C1_M};
    la::Matrix<std::complex<float>> R_RES{{{{-5.f, 0.f}, {-1.4f, 0.f}, {0.f, 0.f}, {4.8f, 0.f}}}, C1_N, C1_M};
    CHECK_NOTHROW(QR.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "QR factorization of complex<float> matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "Q " << QR.Q().GetRowsNb() << " x " << QR.Q().GetColsNb() << "\n";
        std::cout << QR.Q();
        std::cout << "R " << QR.R().GetRowsNb() << " x " << QR.R().GetColsNb() << "\n";
        std::cout << QR.R();
        std::cout << "A - Q * R \n";
        std::cout << A - QR.C();
        std::cout << "Q * R \n";
        std::cout << QR.C();
    }
    for (size_t i = 0; i < QR.Q().GetRowsNb(); i++)
        for (size_t j = 0; j < QR.Q().GetColsNb(); j++)
        {
            REQUIRE(fabs(QR.Q()[i][j].real() - Q_RES[i][j].real()) < 1e-5);
            REQUIRE(fabs(QR.Q()[i][j].imag() - Q_RES[i][j].imag()) < 1e-5);
        }
    for (size_t i = 0; i < QR.R().GetRowsNb(); i++)
        for (size_t j = 0; j < QR.R().GetColsNb(); j++)
        {
            REQUIRE(fabs(QR.R()[i][j].real() - R_RES[i][j].real()) < 1e-5);
            REQUIRE(fabs(QR.R()[i][j].imag() - R_RES[i][j].imag()) < 1e-5);
        }
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++)
        {
            REQUIRE(fabs(A[i][j].real() - QR.C()[i][j].real()) < 1e-5);
            REQUIRE(fabs(A[i][j].imag() - QR.C()[i][j].imag()) < 1e-5);
        }
}

TEST_CASE("[lapack][QR] complex<double>")
{
    la::Matrix<std::complex<double>> A{MCOMPL1, C1_N, C1_M}, Q{C1_N, C1_N};
    int flags = 0;
    la::MatrixQR<std::complex<double>> QR{A};
    la::Matrix<std::complex<double>> Q_RES{{{{-0.8, 0}, {0.6, 0}, {0.6, 0}, {0.8, 0}}}, C1_N, C1_M};
    la::Matrix<std::complex<double>> R_RES{{{{-5, 0}, {-1.4, 0}, {0, 0}, {4.8, 0}}}, C1_N, C1_M};
    CHECK_NOTHROW(QR.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "QR factorization of complex<double> matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "Q " << QR.Q().GetRowsNb() << " x " << QR.Q().GetColsNb() << "\n";
        std::cout << QR.Q();
        std::cout << "R " << QR.R().GetRowsNb() << " x " << QR.R().GetColsNb() << "\n";
        std::cout << QR.R();
        std::cout << "A - Q * R \n";
        std::cout << A - QR.C();
        std::cout << "Q * R \n";
        std::cout << QR.C();
    }
    for (size_t i = 0; i < QR.Q().GetRowsNb(); i++)
        for (size_t j = 0; j < QR.Q().GetColsNb(); j++)
        {
            REQUIRE(fabs(QR.Q()[i][j].real() - Q_RES[i][j].real()) < 1e-9);
            REQUIRE(fabs(QR.Q()[i][j].imag() - Q_RES[i][j].imag()) < 1e-9);
        }
    for (size_t i = 0; i < QR.R().GetRowsNb(); i++)
        for (size_t j = 0; j < QR.R().GetColsNb(); j++)
        {
            REQUIRE(fabs(QR.R()[i][j].real() - R_RES[i][j].real()) < 1e-9);
            REQUIRE(fabs(QR.R()[i][j].imag() - R_RES[i][j].imag()) < 1e-9);
        }
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++)
        {
            REQUIRE(fabs(A[i][j].real() - QR.C()[i][j].real()) < 1e-9);
            REQUIRE(fabs(A[i][j].imag() - QR.C()[i][j].imag()) < 1e-9);
        }
}

TEST_SUITE_END();

#endif
