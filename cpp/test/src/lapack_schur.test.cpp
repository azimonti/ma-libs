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
#include "algebra/la_lapack_schur.h"
#include "std/cout.h"
#include "doctest.h"
#include "global_static.h"

TEST_SUITE_BEGIN("[LAPACK][Schur]");

TEST_CASE("[lapack][Schur] float")
{
    la::Matrix<float> A{
        {4, 4, -3, 3},
        2, 2
    },
        E_RES({3.5f, 3.42783f, 3.5f, -3.42783f}, 2, 2), S_RES({3.5f, 4.20711f, -2.79289f, 3.5f}, 2, 2),
        V_RES({0.92388f, 0.382683f, -0.382683f, 0.92388f}, 2, 2);
    la::MatrixSchur<float> Schur(A);
    int flags = la::SCHUR::COMPUTE_V;
    CHECK_NOTHROW(Schur.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Schur decomposition of float matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Schur.E().GetRowsNb() << " x " << Schur.E().GetColsNb() << "\n";
        std::cout << Schur.E();
        std::cout << "S " << Schur.S().GetRowsNb() << " x " << Schur.S().GetColsNb() << "\n";
        std::cout << Schur.S();
        std::cout << "V " << Schur.V().GetRowsNb() << " x " << Schur.V().GetColsNb() << "\n";
        std::cout << Schur.V();
        std::cout << "V * S * (V**T) " << Schur.C().GetRowsNb() << " x " << Schur.C().GetColsNb() << "\n";
        std::cout << Schur.C();
    }
    for (size_t i = 0; i < Schur.E().GetRowsNb(); i++)
        for (size_t j = 0; j < 2; j++) REQUIRE(fabs(Schur.E()[i][j] - E_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < Schur.S().GetRowsNb(); i++)
        for (size_t j = 0; j < Schur.S().GetColsNb(); j++) REQUIRE(fabs(Schur.S()[i][j] - S_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < Schur.V().GetRowsNb(); i++)
        for (size_t j = 0; j < Schur.V().GetColsNb(); j++) REQUIRE(fabs(Schur.V()[i][j] - V_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(A[i][j] - Schur.C()[i][j]) < 1e-5);
}

TEST_CASE("[lapack][Schur] double 1")
{
    la::Matrix<double> A{
        {4, 4, -3, 3},
        2, 2
    },
        E_RES({3.5, 3.42783, 3.5, -3.42783}, 2, 2), S_RES({3.5, 4.20711, -2.79289, 3.5}, 2, 2),
        V_RES({0.92388, 0.382683, -0.382683, 0.92388}, 2, 2);
    la::MatrixSchur<double> Schur(A);
    int flags = la::SCHUR::COMPUTE_V;
    CHECK_NOTHROW(Schur.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Schur decomposition of double matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Schur.E().GetRowsNb() << " x " << Schur.E().GetColsNb() << "\n";
        std::cout << Schur.E();
        std::cout << "S " << Schur.S().GetRowsNb() << " x " << Schur.S().GetColsNb() << "\n";
        std::cout << Schur.S();
        std::cout << "V " << Schur.V().GetRowsNb() << " x " << Schur.V().GetColsNb() << "\n";
        std::cout << Schur.V();
        std::cout << "V * S * (V**T) " << Schur.C().GetRowsNb() << " x " << Schur.C().GetColsNb() << "\n";
        std::cout << Schur.C();
    }
    for (size_t i = 0; i < Schur.E().GetRowsNb(); i++)
        for (size_t j = 0; j < 2; j++) REQUIRE(fabs(Schur.E()[i][j] - E_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < Schur.S().GetRowsNb(); i++)
        for (size_t j = 0; j < Schur.S().GetColsNb(); j++) REQUIRE(fabs(Schur.S()[i][j] - S_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < Schur.V().GetRowsNb(); i++)
        for (size_t j = 0; j < Schur.V().GetColsNb(); j++) REQUIRE(fabs(Schur.V()[i][j] - V_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(A[i][j] - Schur.C()[i][j]) < 1e-5);
}

TEST_CASE("[lapack][Schur] singular double 2")
{
    la::Matrix<double> A{
        {1, 2, 3, 2, 4, 6, 6, -2, 4},
        3, 3
    },
        E_RES({7, 0, 0, 0, 2, 0}, 3, 2), S_RES({7, -4.94975, 3.53553, 0, 6.93889e-18, -6, 0, 0, 2}, 3, 3),
        V_RES({0.428571, 0.404061, 0.808122, 0.857143, 0.101015, -0.505076, 0.285714, -0.909137, 0.303046}, 3, 3);
    la::MatrixSchur<double> Schur(A);
    int flags = la::SCHUR::COMPUTE_V;
    CHECK_NOTHROW(Schur.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Schur decomposition of float matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Schur.E().GetRowsNb() << " x " << Schur.E().GetColsNb() << "\n";
        std::cout << Schur.E();
        std::cout << "S " << Schur.S().GetRowsNb() << " x " << Schur.S().GetColsNb() << "\n";
        std::cout << Schur.S();
        std::cout << "V " << Schur.V().GetRowsNb() << " x " << Schur.V().GetColsNb() << "\n";
        std::cout << Schur.V();
        std::cout << "V * S * (V**T) " << Schur.C().GetRowsNb() << " x " << Schur.C().GetColsNb() << "\n";
        std::cout << Schur.C();
    }
    for (size_t i = 0; i < Schur.E().GetRowsNb(); i++)
        for (size_t j = 0; j < 2; j++) REQUIRE(fabs(Schur.E()[i][j] - E_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < Schur.S().GetRowsNb(); i++)
        for (size_t j = 0; j < Schur.S().GetColsNb(); j++) REQUIRE(fabs(Schur.S()[i][j] - S_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < Schur.V().GetRowsNb(); i++)
        for (size_t j = 0; j < Schur.V().GetColsNb(); j++) REQUIRE(fabs(Schur.V()[i][j] - V_RES[i][j]) < 1e-5);
    for (size_t i = 0; i < A.GetRowsNb(); i++)
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(A[i][j] - Schur.C()[i][j]) < 1e-5);
}

TEST_CASE("[lapack][Schur] std::complex<float>")
{
    la::Matrix<std::complex<float>> A{
        {4, 4, -3, 3},
        2, 2
    },
        E_RES({{3.5f, 3.42783f}, {3.5f, -3.42783f}}, 2, 1),
        S_RES({{3.5, 3.42783f}, {0.739883f, 1.20523f}, {0, 0}, {3.5f, -3.42783f}}, 2, 2),
        V_RES({{-0.109109f, -0.748013f}, {0.652583f, 0.0520315f}, {0.654654f, 0}, {0.168215f, -0.736975f}}, 2, 2);
    la::MatrixSchur<std::complex<float>> Schur(A);
    int flags = la::SCHUR::COMPUTE_V;
    CHECK_NOTHROW(Schur.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Schur decomposition of std::complex<float> matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Schur.E().GetRowsNb() << " x " << Schur.E().GetColsNb() << "\n";
        std::cout << Schur.E();
        std::cout << "S " << Schur.S().GetRowsNb() << " x " << Schur.S().GetColsNb() << "\n";
        std::cout << Schur.S();
        std::cout << "V " << Schur.V().GetRowsNb() << " x " << Schur.V().GetColsNb() << "\n";
        std::cout << Schur.V();
        std::cout << "V * S * (V**T) " << Schur.C().GetRowsNb() << " x " << Schur.C().GetColsNb() << "\n";
        std::cout << Schur.C();
    }
    for (size_t i = 0; i < Schur.E().GetRowsNb(); i++)
    {
        for (size_t j = 0; j < 1; j++) REQUIRE(fabs(Schur.E()[i][j].real() - E_RES[i][j].real()) < 1e-5);
        for (size_t j = 0; j < 1; j++) REQUIRE(fabs(Schur.E()[i][j].imag() - E_RES[i][j].imag()) < 1e-5);
    }
    // same for S and V and A
    for (size_t i = 0; i < Schur.S().GetRowsNb(); i++)
    {
        for (size_t j = 0; j < Schur.S().GetColsNb(); j++)
            REQUIRE(fabs(Schur.S()[i][j].real() - S_RES[i][j].real()) < 1e-5);
        for (size_t j = 0; j < Schur.S().GetColsNb(); j++)
            REQUIRE(fabs(Schur.S()[i][j].imag() - S_RES[i][j].imag()) < 1e-5);
    }
    for (size_t i = 0; i < Schur.V().GetRowsNb(); i++)
    {
        for (size_t j = 0; j < Schur.V().GetColsNb(); j++)
            REQUIRE(fabs(Schur.V()[i][j].real() - V_RES[i][j].real()) < 1e-5);
        for (size_t j = 0; j < Schur.V().GetColsNb(); j++)
            REQUIRE(fabs(Schur.V()[i][j].imag() - V_RES[i][j].imag()) < 1e-5);
    }
    for (size_t i = 0; i < A.GetRowsNb(); i++)
    {
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(A[i][j].real() - Schur.C()[i][j].real()) < 1e-5);
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(A[i][j].imag() - Schur.C()[i][j].imag()) < 1e-5);
    }
}

TEST_CASE("[lapack][Schur] std::complex<double>")
{
    la::Matrix<std::complex<double>> A{
        {4, 4, -3, 3},
        2, 2
    },
        E_RES({{3.5, 3.42783}, {3.5, -3.42783}}, 2, 1),
        S_RES({{3.5, 3.42783}, {0.739883, 1.20523}, {0, 0}, {3.5, -3.42783}}, 2, 2),
        V_RES({{-0.109109, -0.748013}, {0.652583, 0.0520315}, {0.654654, 0}, {0.168215, -0.736975}}, 2, 2);
    la::MatrixSchur<std::complex<double>> Schur(A);
    int flags = la::SCHUR::COMPUTE_V;
    CHECK_NOTHROW(Schur.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Schur decomposition of std::complex<double> matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Schur.E().GetRowsNb() << " x " << Schur.E().GetColsNb() << "\n";
        std::cout << Schur.E();
        std::cout << "S " << Schur.S().GetRowsNb() << " x " << Schur.S().GetColsNb() << "\n";
        std::cout << Schur.S();
        std::cout << "V " << Schur.V().GetRowsNb() << " x " << Schur.V().GetColsNb() << "\n";
        std::cout << Schur.V();
        std::cout << "V * S * (V**T) " << Schur.C().GetRowsNb() << " x " << Schur.C().GetColsNb() << "\n";
        std::cout << Schur.C();
    }
    for (size_t i = 0; i < Schur.E().GetRowsNb(); i++)
    {
        for (size_t j = 0; j < 1; j++) REQUIRE(fabs(Schur.E()[i][j].real() - E_RES[i][j].real()) < 1e-5);
        for (size_t j = 0; j < 1; j++) REQUIRE(fabs(Schur.E()[i][j].imag() - E_RES[i][j].imag()) < 1e-5);
    }
    // same for S and V and A
    for (size_t i = 0; i < Schur.S().GetRowsNb(); i++)
    {
        for (size_t j = 0; j < Schur.S().GetColsNb(); j++)
            REQUIRE(fabs(Schur.S()[i][j].real() - S_RES[i][j].real()) < 1e-5);
        for (size_t j = 0; j < Schur.S().GetColsNb(); j++)
            REQUIRE(fabs(Schur.S()[i][j].imag() - S_RES[i][j].imag()) < 1e-5);
    }
    for (size_t i = 0; i < Schur.V().GetRowsNb(); i++)
    {
        for (size_t j = 0; j < Schur.V().GetColsNb(); j++)
            REQUIRE(fabs(Schur.V()[i][j].real() - V_RES[i][j].real()) < 1e-5);
        for (size_t j = 0; j < Schur.V().GetColsNb(); j++)
            REQUIRE(fabs(Schur.V()[i][j].imag() - V_RES[i][j].imag()) < 1e-5);
    }
    for (size_t i = 0; i < A.GetRowsNb(); i++)
    {
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(A[i][j].real() - Schur.C()[i][j].real()) < 1e-5);
        for (size_t j = 0; j < A.GetColsNb(); j++) REQUIRE(fabs(A[i][j].imag() - Schur.C()[i][j].imag()) < 1e-5);
    }
}

TEST_SUITE_END();

#endif
