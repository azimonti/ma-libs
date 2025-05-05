#ifndef __clang_analyzer__
/**************************/
/*  lapack_misc.test.cpp  */
/*    Version 1.0         */
/*     2023/06/11         */
/**************************/

#include <cassert>
#include <complex>
#include <vector>
#include "algebra/la_blas_mult.h"
#include "algebra/la_lapack_misc.h"
#include "std/cout.h"
#include "doctest.h"
#include "global_static.h"

TEST_SUITE_BEGIN("[LAPACK][Misc]");

TEST_CASE("[lapack][DET] float")
{
    la::Matrix<float> A1{
        {4, 4, -3, 3},
        2, 2
    };
    la::Matrix<float> A2{
        {0, 1, -1, 0},
        2, 2
    };
    la::Matrix<float> A3{
        {1, 2, 3, 2, 4, 6, 7, 8, 9},
        3, 3
    };
    float det1 = la::MatDet(A1), det2 = la::MatDet(A2), det3 = la::MatDet(A3);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Determinant of a float matrix \n";
        std::cout << "A1 " << A1.GetRowsNb() << " x " << A1.GetColsNb() << "\n";
        std::cout << A1 << "\n";
        std::cout << "Det(A1) " << det1 << "\n";
        std::cout << "A2 " << A2.GetRowsNb() << " x " << A2.GetColsNb() << "\n";
        std::cout << A2 << "\n";
        std::cout << "Det(A2) " << det2 << "\n";
        std::cout << A3 << "\n";
        std::cout << "A3 " << A3.GetRowsNb() << " x " << A3.GetColsNb() << "\n";
        std::cout << "Det(A3) " << det3 << "\n";
    }
    REQUIRE(fabs(det1 - 24) < 1e-5);
    REQUIRE(fabs(det2 - 1) < 1e-5);
    REQUIRE(fabs(det3 - 0) < 1e-4);
}

TEST_CASE("[lapack][RANK] float")
{
    la::Matrix<float> A1{
        {4, 4, -3, 3},
        2, 2
    };
    la::Matrix<float> A2{
        {0, 1, -1, 0},
        2, 2
    };
    la::Matrix<float> A3{
        {1, 2, 3, 2, 4, 6, 7, 8, 9},
        3, 3
    };
    size_t rank1 = la::MatRank(A1), rank2 = la::MatRank(A2), rank3 = la::MatRank(A3);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Rank of a float matrix \n";
        std::cout << "A1 " << A1.GetRowsNb() << " x " << A1.GetColsNb() << "\n";
        std::cout << A1 << "\n";
        std::cout << "Rank(A1) " << rank1 << "\n";
        std::cout << "A2 " << A2.GetRowsNb() << " x " << A2.GetColsNb() << "\n";
        std::cout << A2 << "\n";
        std::cout << "Rank(A2) " << rank2 << "\n";
        std::cout << "A3 " << A3.GetRowsNb() << " x " << A3.GetColsNb() << "\n";
        std::cout << A3 << "\n";
        std::cout << "Rank(A3) " << rank3 << "\n";
    }
    REQUIRE(rank1 == 2);
    REQUIRE(rank2 == 2);
    REQUIRE(rank3 == 2);
}

TEST_CASE("[lapack][DET] double")
{
    la::Matrix<double> A1{
        {4, 4, -3, 3},
        2, 2
    };
    la::Matrix<double> A2{
        {0, 1, -1, 0},
        2, 2
    };
    la::Matrix<double> A3{
        {1, 2, 3, 2, 4, 6, 7, 8, 9},
        3, 3
    };
    double det1 = la::MatDet(A1), det2 = la::MatDet(A2), det3 = la::MatDet(A3);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Determinant of a double matrix \n";
        std::cout << "A1 " << A1.GetRowsNb() << " x " << A1.GetColsNb() << "\n";
        std::cout << A1 << "\n";
        std::cout << "Det(A1) " << det1 << "\n";
        std::cout << "A2 " << A2.GetRowsNb() << " x " << A2.GetColsNb() << "\n";
        std::cout << A2 << "\n";
        std::cout << "Det(A2) " << det2 << "\n";
        std::cout << "A3 " << A3.GetRowsNb() << " x " << A3.GetColsNb() << "\n";
        std::cout << A3 << "\n";
        std::cout << "Det(A3) " << det3 << "\n";
    }
    REQUIRE(fabs(det1 - 24) < 1e-5);
    REQUIRE(fabs(det2 - 1) < 1e-5);
    REQUIRE(fabs(det3 - 0) < 1e-5);
}

TEST_CASE("[lapack][RANK] double")
{
    la::Matrix<double> A1{
        {4, 4, -3, 3},
        2, 2
    };
    la::Matrix<double> A2{
        {0, 1, -1, 0},
        2, 2
    };
    la::Matrix<double> A3{
        {1, 2, 3, 2, 4, 6, 7, 8, 9},
        3, 3
    };
    size_t rank1 = la::MatRank(A1), rank2 = la::MatRank(A2), rank3 = la::MatRank(A3);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Rank of a double matrix \n";
        std::cout << "A1 " << A1.GetRowsNb() << " x " << A1.GetColsNb() << "\n";
        std::cout << A1 << "\n";
        std::cout << "Rank(A1) " << rank1 << "\n";
        std::cout << "A2 " << A2.GetRowsNb() << " x " << A2.GetColsNb() << "\n";
        std::cout << A2 << "\n";
        std::cout << "Rank(A2) " << rank2 << "\n";
        std::cout << "A3 " << A3.GetRowsNb() << " x " << A3.GetColsNb() << "\n";
        std::cout << A3 << "\n";
        std::cout << "Rank(A3) " << rank3 << "\n";
    }
    REQUIRE(rank1 == 2);
    REQUIRE(rank2 == 2);
    REQUIRE(rank3 == 2);
}

TEST_CASE("[lapack][RANK] complex<float>")
{
    la::Matrix<std::complex<float>> A1{
        {4, 4, -3, 3},
        2, 2
    };
    la::Matrix<std::complex<float>> A2{
        {0, 1, -1, 0},
        2, 2
    };
    la::Matrix<std::complex<float>> A3{
        {1, 2, 3, 2, 4, 6, 7, 8, 9},
        3, 3
    };
    size_t rank1 = la::MatRank(A1), rank2 = la::MatRank(A2), rank3 = la::MatRank(A3);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Rank of a complex<float> matrix \n";
        std::cout << "A1 " << A1.GetRowsNb() << " x " << A1.GetColsNb() << "\n";
        std::cout << A1 << "\n";
        std::cout << "Rank(A1) " << rank1 << "\n";
        std::cout << "A2 " << A2.GetRowsNb() << " x " << A2.GetColsNb() << "\n";
        std::cout << A2 << "\n";
        std::cout << "Rank(A2) " << rank2 << "\n";
        std::cout << "A3 " << A3.GetRowsNb() << " x " << A3.GetColsNb() << "\n";
        std::cout << A3 << "\n";
        std::cout << "Rank(A3) " << rank3 << "\n";
    }
    REQUIRE(rank1 == 2);
    REQUIRE(rank2 == 2);
    REQUIRE(rank3 == 2);
}

TEST_CASE("[lapack][DET] complex<float>")
{
    la::Matrix<std::complex<float>> A1{
        {4, 4, -3, 3},
        2, 2
    };
    la::Matrix<std::complex<float>> A2{
        {0, 1, -1, 0},
        2, 2
    };
    la::Matrix<std::complex<float>> A3{
        {1, 2, 3, 2, 4, 6, 7, 8, 9},
        3, 3
    };
    std::complex<float> det1 = la::MatDet(A1), det2 = la::MatDet(A2), det3 = la::MatDet(A3);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Determinant of a complex<float> matrix \n";
        std::cout << "A1 " << A1.GetRowsNb() << " x " << A1.GetColsNb() << "\n";
        std::cout << A1 << "\n";
        std::cout << "Det(A1) " << det1.real() << " + " << det1.imag() << "i"
                  << "\n";
        std::cout << "A2 " << A2.GetRowsNb() << " x " << A2.GetColsNb() << "\n";
        std::cout << A2 << "\n";
        std::cout << "Det(A2) " << det2.real() << " + " << det2.imag() << "i"
                  << "\n";
        std::cout << "A3 " << A3.GetRowsNb() << " x " << A3.GetColsNb() << "\n";
        std::cout << A3 << "\n";
        std::cout << "Det(A3) " << det3.real() << " + " << det3.imag() << "i"
                  << "\n";
    }
    REQUIRE(fabs(det1.real() - 24) < 1e-5);
    REQUIRE(fabs(det1.imag() - 0) < 1e-5);
    REQUIRE(fabs(det2.real() - 1) < 1e-5);
    REQUIRE(fabs(det2.imag() - 0) < 1e-5);
    REQUIRE(fabs(det3.real() - 0) < 1e-4);
    REQUIRE(fabs(det3.imag() - 0) < 1e-4);
}

TEST_CASE("[lapack][DET] complex<double>")
{
    la::Matrix<std::complex<double>> A1{
        {4, 4, -3, 3},
        2, 2
    };
    la::Matrix<std::complex<double>> A2{
        {0, 1, -1, 0},
        2, 2
    };
    la::Matrix<std::complex<double>> A3{
        {1, 2, 3, 2, 4, 6, 7, 8, 9},
        3, 3
    };
    la::Matrix<std::complex<double>> A4{
        {{4 - 2}, {4, 1}, {-3, -0.2f}, {1, 3}},
        2, 2
    };
    std::complex<double> det1 = la::MatDet(A1), det2 = la::MatDet(A2), det3 = la::MatDet(A3), det4 = la::MatDet(A4);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Determinant of a complex<double> matrix \n";
        std::cout << "A1 " << A1.GetRowsNb() << " x " << A1.GetColsNb() << "\n";
        std::cout << A1 << "\n";
        std::cout << "Det(A1) " << det1.real() << " + " << det1.imag() << "i"
                  << "\n";
        std::cout << "A2 " << A2.GetRowsNb() << " x " << A2.GetColsNb() << "\n";
        std::cout << A2 << "\n";
        std::cout << "Det(A2) " << det2.real() << " + " << det2.imag() << "i"
                  << "\n";
        std::cout << "A3 " << A3.GetRowsNb() << " x " << A3.GetColsNb() << "\n";
        std::cout << A3 << "\n";
        std::cout << "Det(A3) " << det3.real() << " + " << det3.imag() << "i"
                  << "\n";
        std::cout << "A4 " << A4.GetRowsNb() << " x " << A4.GetColsNb() << "\n";
        std::cout << A4 << "\n";
        std::cout << "Det(A4) " << det4.real() << " + " << det4.imag() << "i"
                  << "\n";
    }
    REQUIRE(fabs(det1.real() - 24) < 1e-5);
    REQUIRE(fabs(det1.imag() - 0) < 1e-5);
    REQUIRE(fabs(det2.real() - 1) < 1e-5);
    REQUIRE(fabs(det2.imag() - 0) < 1e-5);
    REQUIRE(fabs(det3.real() - 0) < 1e-5);
    REQUIRE(fabs(det3.imag() - 0) < 1e-5);
    REQUIRE(fabs(det4.real() - 13.8) < 1e-5);
    REQUIRE(fabs(det4.imag() - 9.8) < 1e-5);
}

TEST_CASE("[lapack][RANK] complex<double>")
{
    la::Matrix<std::complex<double>> A1{
        {4, 4, -3, 3},
        2, 2
    };
    la::Matrix<std::complex<double>> A2{
        {0, 1, -1, 0},
        2, 2
    };
    la::Matrix<std::complex<double>> A3{
        {1, 2, 3, 2, 4, 6, 7, 8, 9},
        3, 3
    };
    la::Matrix<std::complex<double>> A4{
        {{4 - 2}, {4, 1}, {-3, -0.2f}, {1, 3}},
        2, 2
    };
    size_t rank1 = la::MatRank(A1), rank2 = la::MatRank(A2), rank3 = la::MatRank(A3), rank4 = la::MatRank(A4);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Rank of a complex<double> matrix \n";
        std::cout << "A1 " << A1.GetRowsNb() << " x " << A1.GetColsNb() << "\n";
        std::cout << A1 << "\n";
        std::cout << "Rank(A1) " << rank1 << "\n";
        std::cout << "A2 " << A2.GetRowsNb() << " x " << A2.GetColsNb() << "\n";
        std::cout << A2 << "\n";
        std::cout << "Rank(A2) " << rank2 << "\n";
        std::cout << "A3 " << A3.GetRowsNb() << " x " << A3.GetColsNb() << "\n";
        std::cout << A3 << "\n";
        std::cout << "Rank(A3) " << rank3 << "\n";
        std::cout << A4 << "\n";
        std::cout << "Rank(A4) " << rank4 << "\n";
    }
    REQUIRE(rank1 == 2);
    REQUIRE(rank2 == 2);
    REQUIRE(rank3 == 2);
    REQUIRE(rank4 == 2);
}

TEST_SUITE_END();

#endif
