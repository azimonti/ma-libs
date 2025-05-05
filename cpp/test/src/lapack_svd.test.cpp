#ifndef __clang_analyzer__
/**************************/
/*   lapack_svd.test.cpp  */
/*    Version 1.0         */
/*     2023/05/21         */
/**************************/

#include <cassert>
#include <complex>
#include <vector>
#include "algebra/la_blas_mult.h"
#include "algebra/la_lapack_svd.h"
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
TEST_SUITE_BEGIN("[LAPACK][SVD]");

TEST_CASE("[lapack][SVD] float")
{
    la::Matrix<float> A{MREAL1, R1_N, R1_M};
    la::MatrixSVD<float> SVD(A);
    for (int i = la::DRIVER::GESVD; i <= la::DRIVER::GEJSV; ++i)
    {
        for (int j = 0; j <= 1; ++j)
        {
            int flags = 0;
            if (j) flags |= la::SVD::V_HT;
            else flags &= ~la::SVD::V_HT;
            CHECK_NOTHROW(SVD.Compute(i, flags));
            if (GLOBAL_VERBOSE_FLAG)
            {
                std::cout << "SVD of float matrix - driver " << i << " - VT " << j << "\n";
                std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
                std::cout << A;
                std::cout << "U " << SVD.U().GetRowsNb() << " x " << SVD.U().GetColsNb() << "\n";
                std::cout << SVD.U();
                std::cout << "S " << SVD.S().GetRowsNb() << " x " << SVD.S().GetColsNb() << "\n";
                std::cout << SVD.S();
                std::cout << "V " << SVD.V().GetRowsNb() << " x " << SVD.V().GetColsNb() << "\n";
                std::cout << SVD.V();
                std::cout << "A - U * S * V**^T \n";
                std::cout << A - SVD.C();
                std::cout << "U * S * V**^T \n";
                std::cout << SVD.C();
            }
            REQUIRE(fabs(SVD.S()[0][0] - 5.65685) < 1e-4);
            REQUIRE(fabs(SVD.S()[0][1]) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][0]) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][1] - 4.24264) < 1e-4);
            REQUIRE(fabs(A[0][0] - SVD.C()[0][0]) < 1e-5);
            REQUIRE(fabs(A[0][1] - SVD.C()[0][1]) < 1e-5);
            REQUIRE(fabs(A[1][0] - SVD.C()[1][0]) < 1e-5);
            REQUIRE(fabs(A[1][1] - SVD.C()[1][1]) < 1e-5);
            REQUIRE(la::MatIsOrthogonal(SVD.U(), 1e-5f));
            REQUIRE(la::MatIsOrthogonal(SVD.V(), 1e-5f));
        }
    }
}

TEST_CASE("[lapack][SVD] double")
{
    la::Matrix<double> A{MREAL1, R1_N, R1_M}, U{R1_N, R1_N}, S{R1_N, R1_M}, V{R1_M, R1_M}, C{R1_N, R1_M};
    la::MatrixSVD<double> SVD(A);
    for (int i = la::DRIVER::GESVD; i <= la::DRIVER::GEJSV; ++i)
    {
        for (int j = 0; j <= 1; ++j)
        {
            int flags = 0;
            if (j) flags |= la::SVD::V_HT;
            else flags &= ~la::SVD::V_HT;
            CHECK_NOTHROW(SVD.Compute(i, flags));
            if (GLOBAL_VERBOSE_FLAG)
            {
                std::cout << "SVD of double matrix - driver " << i << " - VT " << j << "\n";
                std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
                std::cout << A;
                std::cout << "U " << SVD.U().GetRowsNb() << " x " << SVD.U().GetColsNb() << "\n";
                std::cout << SVD.U();
                std::cout << "S " << SVD.S().GetRowsNb() << " x " << SVD.S().GetColsNb() << "\n";
                std::cout << SVD.S();
                std::cout << "V " << SVD.V().GetRowsNb() << " x " << SVD.V().GetColsNb() << "\n";
                std::cout << SVD.V();
                std::cout << "A - U * S * V**^T \n";
                std::cout << A - SVD.C();
                std::cout << "U * S * V**^T \n";
                std::cout << SVD.C();
            }
            REQUIRE(fabs(SVD.S()[0][0] - 5.65685) < 1e-4);
            REQUIRE(fabs(SVD.S()[0][1]) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][0]) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][1] - 4.24264) < 1e-4);
            REQUIRE(fabs(A[0][0] - SVD.C()[0][0]) < 1e-9);
            REQUIRE(fabs(A[0][1] - SVD.C()[0][1]) < 1e-9);
            REQUIRE(fabs(A[1][0] - SVD.C()[1][0]) < 1e-9);
            REQUIRE(fabs(A[1][1] - SVD.C()[1][1]) < 1e-9);
            REQUIRE(la::MatIsOrthogonal(SVD.U()));
            REQUIRE(la::MatIsOrthogonal(SVD.V()));
        }
    }
}

TEST_CASE("[lapack][SVD] complex<float>> 1")
{
    la::Matrix<std::complex<float>> A{MCOMPL1, C1_N, C1_M}, U{C1_N, C1_N};
    la::MatrixSVD<std::complex<float>> SVD(A);
    for (int i = la::DRIVER::GESVD; i <= la::DRIVER::GEJSV; ++i)
    {
        for (int j = 0; j <= 1; ++j)
        {
            int flags = 0;
            if (j) flags |= la::SVD::V_HT;
            else flags &= ~la::SVD::V_HT;
            CHECK_NOTHROW(SVD.Compute(i, flags));
            if (GLOBAL_VERBOSE_FLAG)
            {
                std::cout << "SVD of complex<float> matrix - driver " << i << " - VT " << j << "\n";
                std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
                std::cout << A;
                std::cout << "U " << SVD.U().GetRowsNb() << " x " << SVD.U().GetColsNb() << "\n";
                std::cout << SVD.U();
                std::cout << "S " << SVD.S().GetRowsNb() << " x " << SVD.S().GetColsNb() << "\n";
                std::cout << SVD.S();
                std::cout << "V " << SVD.V().GetRowsNb() << " x " << SVD.V().GetColsNb() << "\n";
                std::cout << SVD.V();
                std::cout << "A - U * S * V**^T \n";
                std::cout << A - SVD.C();
                std::cout << "U * S * V**^T \n";
                std::cout << SVD.C();
            }
            REQUIRE(fabs(SVD.S()[0][0].real() - 5.65685) < 1e-4);
            REQUIRE(fabs(SVD.S()[0][0].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][1].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][1].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][0].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][0].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][1].real() - 4.24264) < 1e-4);
            REQUIRE(fabs(SVD.S()[1][1].imag()) < 1e-5);
            REQUIRE(fabs(A[0][0].real() - SVD.C()[0][0].real()) < 1e-5);
            REQUIRE(fabs(A[0][0].imag() - SVD.C()[0][0].imag()) < 1e-5);
            REQUIRE(fabs(A[0][1].real() - SVD.C()[0][1].real()) < 1e-5);
            REQUIRE(fabs(A[0][1].imag() - SVD.C()[0][1].imag()) < 1e-5);
            REQUIRE(fabs(A[1][0].real() - SVD.C()[1][0].real()) < 1e-5);
            REQUIRE(fabs(A[1][0].imag() - SVD.C()[1][0].imag()) < 1e-5);
            REQUIRE(fabs(A[1][1].real() - SVD.C()[1][1].real()) < 1e-5);
            REQUIRE(fabs(A[1][1].imag() - SVD.C()[1][1].imag()) < 1e-5);
            std::complex<float> tol = {1e-5f, 0};
            REQUIRE(la::MatIsOrthogonal(SVD.U(), tol));
            REQUIRE(la::MatIsOrthogonal(SVD.V(), tol));
        }
    }
}

TEST_CASE("[lapack][SVD] complex<float>> 2")
{
    la::Matrix<std::complex<float>> A{MCOMPL2F, C2_N, C2_M};
    la::MatrixSVD<std::complex<float>> SVD(A);
    for (int i = la::DRIVER::GESVD; i <= la::DRIVER::GEJSV; ++i)
    {
        for (int j = 0; j <= 1; ++j)
        {
            int flags = 0;
            if (j) flags |= la::SVD::V_HT;
            else flags &= ~la::SVD::V_HT;
            flags |= la::SVD::COMPLETE_U;
            flags |= la::SVD::COMPLETE_V;
            CHECK_NOTHROW(SVD.Compute(i, flags));
            if (GLOBAL_VERBOSE_FLAG)
            {
                std::cout << "SVD of complex<float> matrix - driver " << i << " - VT " << j << "\n";
                std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
                std::cout << A;
                std::cout << "U " << SVD.U().GetRowsNb() << " x " << SVD.U().GetColsNb() << "\n";
                std::cout << SVD.U();
                std::cout << "S " << SVD.S().GetRowsNb() << " x " << SVD.S().GetColsNb() << "\n";
                std::cout << SVD.S();
                std::cout << "V " << SVD.V().GetRowsNb() << " x " << SVD.V().GetColsNb() << "\n";
                std::cout << SVD.V();
                std::cout << "A - U * S * V**^T \n";
                std::cout << A - SVD.C();
            }
            REQUIRE(fabs(SVD.S()[0][0].real() - 17.6254) < 1e-4);
            REQUIRE(fabs(SVD.S()[0][0].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][1].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][1].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][2].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][2].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][3].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][3].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][0].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][0].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][1].real() - 11.6102) < 1e-4);
            REQUIRE(fabs(SVD.S()[1][1].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][2].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][2].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][3].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][3].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[2][0].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[2][0].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[2][1].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[2][1].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[2][2].real() - 6.78285) < 1e-4);
            REQUIRE(fabs(SVD.S()[2][2].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[2][3].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[2][3].imag()) < 1e-5);
            REQUIRE(fabs(A[0][0].real() - SVD.C()[0][0].real()) < 1e-5);
            REQUIRE(fabs(A[0][0].imag() - SVD.C()[0][0].imag()) < 1e-5);
            REQUIRE(fabs(A[0][1].real() - SVD.C()[0][1].real()) < 1e-5);
            REQUIRE(fabs(A[0][1].imag() - SVD.C()[0][1].imag()) < 1e-5);
            REQUIRE(fabs(A[0][2].real() - SVD.C()[0][2].real()) < 1e-5);
            REQUIRE(fabs(A[0][2].imag() - SVD.C()[0][2].imag()) < 1e-5);
            REQUIRE(fabs(A[0][3].real() - SVD.C()[0][3].real()) < 1e-5);
            REQUIRE(fabs(A[0][3].imag() - SVD.C()[0][3].imag()) < 1e-5);
            REQUIRE(fabs(A[1][0].real() - SVD.C()[1][0].real()) < 1e-5);
            REQUIRE(fabs(A[1][0].imag() - SVD.C()[1][0].imag()) < 1e-5);
            REQUIRE(fabs(A[1][1].real() - SVD.C()[1][1].real()) < 1e-5);
            REQUIRE(fabs(A[1][1].imag() - SVD.C()[1][1].imag()) < 1e-5);
            REQUIRE(fabs(A[1][2].real() - SVD.C()[1][2].real()) < 1e-5);
            REQUIRE(fabs(A[1][2].imag() - SVD.C()[1][2].imag()) < 1e-5);
            REQUIRE(fabs(A[1][3].real() - SVD.C()[1][3].real()) < 1e-5);
            REQUIRE(fabs(A[1][3].imag() - SVD.C()[1][3].imag()) < 1e-5);
            REQUIRE(fabs(A[2][0].real() - SVD.C()[2][0].real()) < 1e-5);
            REQUIRE(fabs(A[2][0].imag() - SVD.C()[2][0].imag()) < 1e-5);
            REQUIRE(fabs(A[2][1].real() - SVD.C()[2][1].real()) < 1e-5);
            REQUIRE(fabs(A[2][1].imag() - SVD.C()[2][1].imag()) < 1e-5);
            REQUIRE(fabs(A[2][2].real() - SVD.C()[2][2].real()) < 1e-5);
            REQUIRE(fabs(A[2][2].imag() - SVD.C()[2][2].imag()) < 1e-5);
            REQUIRE(fabs(A[2][3].real() - SVD.C()[2][3].real()) < 1e-5);
            REQUIRE(fabs(A[2][3].imag() - SVD.C()[2][3].imag()) < 1e-5);
            std::complex<float> tol = {1e-5f, 0};
            REQUIRE(la::MatIsOrthogonal(SVD.U(), tol));
            REQUIRE(la::MatIsOrthogonal(SVD.V(), tol));
        }
    }
}

TEST_CASE("[lapack][SVD] complex<float>> 3")
{
    la::Matrix<std::complex<float>> A{MCOMPL2F, C2_N, C2_M};
    A.Transpose();
    la::MatrixSVD<std::complex<float>> SVD(A);
    for (int i = la::DRIVER::GESVD; i <= la::DRIVER::GEJSV; ++i)
    {
        for (int j = 0; j <= 1; ++j)
        {
            int flags = 0;
            if (j) flags |= la::SVD::V_HT;
            else flags &= ~la::SVD::V_HT;
            flags |= la::SVD::COMPLETE_U;
            flags |= la::SVD::COMPLETE_V;
            CHECK_NOTHROW(SVD.Compute(i, flags));
            if (GLOBAL_VERBOSE_FLAG)
            {
                std::cout << "SVD of complex<float> matrix - driver " << i << " - VT " << j << "\n";
                std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
                std::cout << A;
                std::cout << "U " << SVD.U().GetRowsNb() << " x " << SVD.U().GetColsNb() << "\n";
                std::cout << SVD.U();
                std::cout << "S " << SVD.S().GetRowsNb() << " x " << SVD.S().GetColsNb() << "\n";
                std::cout << SVD.S();
                std::cout << "V " << SVD.V().GetRowsNb() << " x " << SVD.V().GetColsNb() << "\n";
                std::cout << SVD.V();
                std::cout << "A - U * S * V**^T \n";
                std::cout << A - SVD.C();
            }
            REQUIRE(fabs(SVD.S()[0][0].real() - 17.6254) < 1e-4);
            REQUIRE(fabs(SVD.S()[0][0].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][1].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][1].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][2].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][2].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][0].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][0].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][1].real() - 11.6102) < 1e-4);
            REQUIRE(fabs(SVD.S()[1][1].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][2].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][2].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[2][0].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[2][0].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[2][1].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[2][1].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[2][2].real() - 6.78285) < 1e-4);
            REQUIRE(fabs(SVD.S()[2][2].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[3][0].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[3][0].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[3][1].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[3][1].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[3][2].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[3][2].imag()) < 1e-5);
            REQUIRE(fabs(A[0][0].real() - SVD.C()[0][0].real()) < 1e-5);
            REQUIRE(fabs(A[0][0].imag() - SVD.C()[0][0].imag()) < 1e-5);
            REQUIRE(fabs(A[0][1].real() - SVD.C()[0][1].real()) < 1e-5);
            REQUIRE(fabs(A[0][1].imag() - SVD.C()[0][1].imag()) < 1e-5);
            REQUIRE(fabs(A[0][2].real() - SVD.C()[0][2].real()) < 1e-5);
            REQUIRE(fabs(A[0][2].imag() - SVD.C()[0][2].imag()) < 1e-5);
            REQUIRE(fabs(A[1][0].real() - SVD.C()[1][0].real()) < 1e-5);
            REQUIRE(fabs(A[1][0].imag() - SVD.C()[1][0].imag()) < 1e-5);
            REQUIRE(fabs(A[1][1].real() - SVD.C()[1][1].real()) < 1e-5);
            REQUIRE(fabs(A[1][1].imag() - SVD.C()[1][1].imag()) < 1e-5);
            REQUIRE(fabs(A[1][2].real() - SVD.C()[1][2].real()) < 1e-5);
            REQUIRE(fabs(A[1][2].imag() - SVD.C()[1][2].imag()) < 1e-5);
            REQUIRE(fabs(A[2][0].real() - SVD.C()[2][0].real()) < 1e-5);
            REQUIRE(fabs(A[2][0].imag() - SVD.C()[2][0].imag()) < 1e-5);
            REQUIRE(fabs(A[2][1].real() - SVD.C()[2][1].real()) < 1e-5);
            REQUIRE(fabs(A[2][1].imag() - SVD.C()[2][1].imag()) < 1e-5);
            REQUIRE(fabs(A[2][2].real() - SVD.C()[2][2].real()) < 1e-5);
            REQUIRE(fabs(A[2][2].imag() - SVD.C()[2][2].imag()) < 1e-5);
            REQUIRE(fabs(A[3][0].real() - SVD.C()[3][0].real()) < 1e-5);
            REQUIRE(fabs(A[3][0].imag() - SVD.C()[3][0].imag()) < 1e-5);
            REQUIRE(fabs(A[3][1].real() - SVD.C()[3][1].real()) < 1e-5);
            REQUIRE(fabs(A[3][1].imag() - SVD.C()[3][1].imag()) < 1e-5);
            REQUIRE(fabs(A[3][2].real() - SVD.C()[3][2].real()) < 1e-5);
            REQUIRE(fabs(A[3][2].imag() - SVD.C()[3][2].imag()) < 1e-5);
            std::complex<float> tol = {1e-5f, 0};
            REQUIRE(la::MatIsOrthogonal(SVD.U(), tol));
            REQUIRE(la::MatIsOrthogonal(SVD.V(), tol));
        }
    }
}

TEST_CASE("[lapack][SVD] complex<double>> 1")
{
    la::Matrix<std::complex<double>> A{MCOMPL1, C1_N, C1_M};
    la::MatrixSVD<std::complex<double>> SVD(A);
    for (int i = la::DRIVER::GESVD; i <= la::DRIVER::GEJSV; ++i)
    {
        for (int j = 0; j <= 1; ++j)
        {
            int flags = 0;
            if (j) flags |= la::SVD::V_HT;
            else flags &= ~la::SVD::V_HT;
            flags |= la::SVD::COMPLETE_U;
            flags |= la::SVD::COMPLETE_V;
            CHECK_NOTHROW(SVD.Compute(i, flags));
            if (GLOBAL_VERBOSE_FLAG)
            {
                std::cout << "SVD of complex<double> matrix - driver " << i << " - VT " << j << "\n";
                std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
                std::cout << A;
                std::cout << "U " << SVD.U().GetRowsNb() << " x " << SVD.U().GetColsNb() << "\n";
                std::cout << SVD.U();
                std::cout << "S " << SVD.S().GetRowsNb() << " x " << SVD.S().GetColsNb() << "\n";
                std::cout << SVD.S();
                std::cout << "V " << SVD.V().GetRowsNb() << " x " << SVD.V().GetColsNb() << "\n";
                std::cout << SVD.V();
                std::cout << "A - U * S * V**^T \n";
                std::cout << A - SVD.C();
                std::cout << "U * S * V**^T \n";
                std::cout << SVD.C();
            }
            REQUIRE(fabs(SVD.S()[0][0].real() - 5.65685) < 1e-4);
            REQUIRE(fabs(SVD.S()[0][0].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][1].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[0][1].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][0].real()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][0].imag()) < 1e-5);
            REQUIRE(fabs(SVD.S()[1][1].real() - 4.24264) < 1e-4);
            REQUIRE(fabs(SVD.S()[1][1].imag()) < 1e-5);
            REQUIRE(fabs(A[0][0].real() - SVD.C()[0][0].real()) < 1e-5);
            REQUIRE(fabs(A[0][0].imag() - SVD.C()[0][0].imag()) < 1e-5);
            REQUIRE(fabs(A[0][1].real() - SVD.C()[0][1].real()) < 1e-5);
            REQUIRE(fabs(A[0][1].imag() - SVD.C()[0][1].imag()) < 1e-5);
            REQUIRE(fabs(A[1][0].real() - SVD.C()[1][0].real()) < 1e-5);
            REQUIRE(fabs(A[1][0].imag() - SVD.C()[1][0].imag()) < 1e-5);
            REQUIRE(fabs(A[1][1].real() - SVD.C()[1][1].real()) < 1e-5);
            REQUIRE(fabs(A[1][1].imag() - SVD.C()[1][1].imag()) < 1e-5);
            REQUIRE(la::MatIsOrthogonal(SVD.U()));
            REQUIRE(la::MatIsOrthogonal(SVD.V()));
        }
    }
}

TEST_CASE("[lapack][SVD] complex<double>> 2")
{
    la::Matrix<std::complex<double>> A{MCOMPL2D, C2_N, C2_M};
    la::MatrixSVD<std::complex<double>> SVD(A);
    for (int i = la::DRIVER::GESVD; i <= la::DRIVER::GEJSV; ++i)
    {
        for (int j = 0; j <= 1; ++j)
        {
            int flags = 0;
            if (j) flags |= la::SVD::V_HT;
            else flags &= ~la::SVD::V_HT;
            flags |= la::SVD::COMPLETE_U;
            flags |= la::SVD::COMPLETE_V;
            CHECK_NOTHROW(SVD.Compute(i, flags));
            if (GLOBAL_VERBOSE_FLAG)
            {
                std::cout << "SVD of complex<double> matrix - driver " << i << " - VT " << j << "\n";
                std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
                std::cout << A;
                std::cout << "U " << SVD.U().GetRowsNb() << " x " << SVD.U().GetColsNb() << "\n";
                std::cout << SVD.U();
                std::cout << "S " << SVD.S().GetRowsNb() << " x " << SVD.S().GetColsNb() << "\n";
                std::cout << SVD.S();
                std::cout << "V " << SVD.V().GetRowsNb() << " x " << SVD.V().GetColsNb() << "\n";
                std::cout << SVD.V();
                std::cout << "A - U * S * V**^T \n";
                std::cout << A - SVD.C();
                std::cout << "U * S * V**^T \n";
                std::cout << SVD.C();
            }
            REQUIRE(fabs(SVD.S()[0][0].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[0][1].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[0][1].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[0][2].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[0][2].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[0][3].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[0][3].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][0].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][0].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][1].real() - 11.6102) < 1e-4);
            REQUIRE(fabs(SVD.S()[1][1].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][2].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][2].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][3].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][3].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[2][0].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[2][0].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[2][1].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[2][1].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[2][2].real() - 6.78285) < 1e-4);
            REQUIRE(fabs(SVD.S()[2][2].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[2][3].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[2][3].imag()) < 1e-9);
            REQUIRE(fabs(A[0][0].real() - SVD.C()[0][0].real()) < 1e-9);
            REQUIRE(fabs(A[0][0].imag() - SVD.C()[0][0].imag()) < 1e-9);
            REQUIRE(fabs(A[0][1].real() - SVD.C()[0][1].real()) < 1e-9);
            REQUIRE(fabs(A[0][1].imag() - SVD.C()[0][1].imag()) < 1e-9);
            REQUIRE(fabs(A[0][2].real() - SVD.C()[0][2].real()) < 1e-9);
            REQUIRE(fabs(A[0][2].imag() - SVD.C()[0][2].imag()) < 1e-9);
            REQUIRE(fabs(A[0][3].real() - SVD.C()[0][3].real()) < 1e-9);
            REQUIRE(fabs(A[0][3].imag() - SVD.C()[0][3].imag()) < 1e-9);
            REQUIRE(fabs(A[1][0].real() - SVD.C()[1][0].real()) < 1e-9);
            REQUIRE(fabs(A[1][0].imag() - SVD.C()[1][0].imag()) < 1e-9);
            REQUIRE(fabs(A[1][1].real() - SVD.C()[1][1].real()) < 1e-9);
            REQUIRE(fabs(A[1][1].imag() - SVD.C()[1][1].imag()) < 1e-9);
            REQUIRE(fabs(A[1][2].real() - SVD.C()[1][2].real()) < 1e-9);
            REQUIRE(fabs(A[1][2].imag() - SVD.C()[1][2].imag()) < 1e-9);
            REQUIRE(fabs(A[1][3].real() - SVD.C()[1][3].real()) < 1e-9);
            REQUIRE(fabs(A[1][3].imag() - SVD.C()[1][3].imag()) < 1e-9);
            REQUIRE(fabs(A[2][0].real() - SVD.C()[2][0].real()) < 1e-9);
            REQUIRE(fabs(A[2][0].imag() - SVD.C()[2][0].imag()) < 1e-9);
            REQUIRE(fabs(A[2][1].real() - SVD.C()[2][1].real()) < 1e-9);
            REQUIRE(fabs(A[2][1].imag() - SVD.C()[2][1].imag()) < 1e-9);
            REQUIRE(fabs(A[2][2].real() - SVD.C()[2][2].real()) < 1e-9);
            REQUIRE(fabs(A[2][2].imag() - SVD.C()[2][2].imag()) < 1e-9);
            REQUIRE(fabs(A[2][3].real() - SVD.C()[2][3].real()) < 1e-9);
            REQUIRE(fabs(A[2][3].imag() - SVD.C()[2][3].imag()) < 1e-9);
            REQUIRE(la::MatIsOrthogonal(SVD.U()));
            REQUIRE(la::MatIsOrthogonal(SVD.V()));
        }
    }
}

TEST_CASE("[lapack][SVD] complex<double>> 3")
{
    la::Matrix<std::complex<double>> A{MCOMPL2D, C2_N, C2_M};
    A.Transpose();
    la::MatrixSVD<std::complex<double>> SVD(A);
    for (int i = la::DRIVER::GESVD; i <= la::DRIVER::GEJSV; ++i)
    {
        for (int j = 0; j <= 1; ++j)
        {
            int flags = 0;
            if (j) flags |= la::SVD::V_HT;
            else flags &= ~la::SVD::V_HT;
            flags |= la::SVD::COMPLETE_U;
            flags |= la::SVD::COMPLETE_V;
            CHECK_NOTHROW(SVD.Compute(i, flags));
            if (GLOBAL_VERBOSE_FLAG)
            {
                std::cout << "SVD of complex<double> matrix - driver " << i << " - VT " << j << "\n";
                std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
                std::cout << A;
                std::cout << "U " << SVD.U().GetRowsNb() << " x " << SVD.U().GetColsNb() << "\n";
                std::cout << SVD.U();
                std::cout << "S " << SVD.S().GetRowsNb() << " x " << SVD.S().GetColsNb() << "\n";
                std::cout << SVD.S();
                std::cout << "V " << SVD.V().GetRowsNb() << " x " << SVD.V().GetColsNb() << "\n";
                std::cout << SVD.V();
                std::cout << "A - U * S * V**^T \n";
                std::cout << A - SVD.C();
                std::cout << "U * S * V**^T \n";
                std::cout << SVD.C();
            }
            REQUIRE(fabs(SVD.S()[0][0].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[0][1].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[0][1].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[0][2].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[0][2].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][0].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][0].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][1].real() - 11.6102) < 1e-4);
            REQUIRE(fabs(SVD.S()[1][1].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][2].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[1][2].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[2][0].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[2][0].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[2][1].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[2][1].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[2][2].real() - 6.78285) < 1e-4);
            REQUIRE(fabs(SVD.S()[2][2].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[3][0].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[3][0].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[3][1].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[3][1].imag()) < 1e-9);
            REQUIRE(fabs(SVD.S()[3][2].real()) < 1e-9);
            REQUIRE(fabs(SVD.S()[3][2].imag()) < 1e-9);
            REQUIRE(fabs(A[0][0].real() - SVD.C()[0][0].real()) < 1e-9);
            REQUIRE(fabs(A[0][0].imag() - SVD.C()[0][0].imag()) < 1e-9);
            REQUIRE(fabs(A[0][1].real() - SVD.C()[0][1].real()) < 1e-9);
            REQUIRE(fabs(A[0][1].imag() - SVD.C()[0][1].imag()) < 1e-9);
            REQUIRE(fabs(A[0][2].real() - SVD.C()[0][2].real()) < 1e-9);
            REQUIRE(fabs(A[0][2].imag() - SVD.C()[0][2].imag()) < 1e-9);
            REQUIRE(fabs(A[1][0].real() - SVD.C()[1][0].real()) < 1e-9);
            REQUIRE(fabs(A[1][0].imag() - SVD.C()[1][0].imag()) < 1e-9);
            REQUIRE(fabs(A[1][1].real() - SVD.C()[1][1].real()) < 1e-9);
            REQUIRE(fabs(A[1][1].imag() - SVD.C()[1][1].imag()) < 1e-9);
            REQUIRE(fabs(A[1][2].real() - SVD.C()[1][2].real()) < 1e-9);
            REQUIRE(fabs(A[1][2].imag() - SVD.C()[1][2].imag()) < 1e-9);
            REQUIRE(fabs(A[2][0].real() - SVD.C()[2][0].real()) < 1e-9);
            REQUIRE(fabs(A[2][0].imag() - SVD.C()[2][0].imag()) < 1e-9);
            REQUIRE(fabs(A[2][1].real() - SVD.C()[2][1].real()) < 1e-9);
            REQUIRE(fabs(A[2][1].imag() - SVD.C()[2][1].imag()) < 1e-9);
            REQUIRE(fabs(A[2][2].real() - SVD.C()[2][2].real()) < 1e-9);
            REQUIRE(fabs(A[2][2].imag() - SVD.C()[2][2].imag()) < 1e-9);
            REQUIRE(fabs(A[3][0].real() - SVD.C()[3][0].real()) < 1e-9);
            REQUIRE(fabs(A[3][0].imag() - SVD.C()[3][0].imag()) < 1e-9);
            REQUIRE(fabs(A[3][1].real() - SVD.C()[3][1].real()) < 1e-9);
            REQUIRE(fabs(A[3][1].imag() - SVD.C()[3][1].imag()) < 1e-9);
            REQUIRE(fabs(A[3][2].real() - SVD.C()[3][2].real()) < 1e-9);
            REQUIRE(fabs(A[3][2].imag() - SVD.C()[3][2].imag()) < 1e-9);
            REQUIRE(la::MatIsOrthogonal(SVD.U()));
            REQUIRE(la::MatIsOrthogonal(SVD.V()));
        }
    }
}

TEST_SUITE_END();

#endif
