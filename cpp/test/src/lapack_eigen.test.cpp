#ifndef __clang_analyzer__
/**************************/
/* lapack_eigen.test.cpp  */
/*    Version 1.0         */
/*     2023/06/11         */
/**************************/

#include <cassert>
#include <complex>
#include <vector>
#include "algebra/la_blas_mult.h"
#include "algebra/la_lapack_eigen.h"
#include "algebra/matrix.h"
#include "algebra/matrix_operations.h"
#include "std/cout.h"
#include "doctest.h"
#include "global_static.h"

TEST_SUITE_BEGIN("[LAPACK][EIGEN]");

TEST_CASE("[lapack][EIGEN] float 1")
{
    la::Matrix<float> A{
        {4.f, 4.f, -3.f, 3.f},
        2, 2
    };
    int flags = 0;
    flags |= la::EIGEN::COMPUTE_VL;
    flags |= la::EIGEN::COMPUTE_VR;
    la::MatrixEigen<float> Eigen{A};
    CHECK_NOTHROW(Eigen.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Eigenvalues and eigenvectors of float matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Eigen.E().GetRowsNb() << " x " << Eigen.E().GetColsNb() << "\n";
        std::cout << Eigen.E();
        std::cout << "EC " << Eigen.EC().GetRowsNb() << " x " << Eigen.EC().GetColsNb() << "\n";
        std::cout << Eigen.EC();
        std::cout << "VL " << Eigen.VL().GetRowsNb() << " x " << Eigen.VL().GetColsNb() << "\n";
        std::cout << Eigen.VL();
        std::cout << "VLC" << Eigen.VLC().GetRowsNb() << " x " << Eigen.VLC().GetColsNb() << "\n";
        std::cout << Eigen.VLC();
        std::cout << "VR " << Eigen.VR().GetRowsNb() << " x " << Eigen.VR().GetColsNb() << "\n";
        std::cout << Eigen.VR();
        std::cout << "VRC" << Eigen.VRC().GetRowsNb() << " x " << Eigen.VRC().GetColsNb() << "\n";
        std::cout << Eigen.VRC();
        std::cout << "A*VRC\n";
        std::cout << Eigen.AV(true);
        std::cout << "E*VRC\n";
        std::cout << Eigen.EV(true);
        std::cout << "VLC**H*A\n";
        std::cout << Eigen.AV(false);
        std::cout << "VLC**H*E\n";
        std::cout << Eigen.EV(false);
    }
    for (int k = 0; k <= 1; ++k)
        for (size_t i = 0; i < Eigen.AV(k).GetRowsNb(); i++)
            for (size_t j = 0; j < Eigen.AV(k).GetColsNb(); j++)
            {
                REQUIRE(fabs(Eigen.AV(k)[i][j].real() - Eigen.EV(k)[i][j].real()) < 1e-5);
                REQUIRE(fabs(Eigen.AV(k)[i][j].imag() - Eigen.EV(k)[i][j].imag()) < 1e-5);
            }
}

TEST_CASE("[lapack][EIGEN] float 2")
{
    la::Matrix<float> A{
        {0, 1, -1, 0},
        2, 2
    };
    int flags = 0;
    flags |= la::EIGEN::COMPUTE_VL;
    flags |= la::EIGEN::COMPUTE_VR;
    la::MatrixEigen<float> Eigen{A};
    CHECK_NOTHROW(Eigen.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Eigenvalues and eigenvectors of float matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Eigen.E().GetRowsNb() << " x " << Eigen.E().GetColsNb() << "\n";
        std::cout << Eigen.E();
        std::cout << "EC " << Eigen.EC().GetRowsNb() << " x " << Eigen.EC().GetColsNb() << "\n";
        std::cout << Eigen.EC();
        std::cout << "VL " << Eigen.VL().GetRowsNb() << " x " << Eigen.VL().GetColsNb() << "\n";
        std::cout << Eigen.VL();
        std::cout << "VLC" << Eigen.VLC().GetRowsNb() << " x " << Eigen.VLC().GetColsNb() << "\n";
        std::cout << Eigen.VLC();
        std::cout << "VR " << Eigen.VR().GetRowsNb() << " x " << Eigen.VR().GetColsNb() << "\n";
        std::cout << Eigen.VR();
        std::cout << "VRC" << Eigen.VRC().GetRowsNb() << " x " << Eigen.VRC().GetColsNb() << "\n";
        std::cout << Eigen.VRC();
        std::cout << "A*VRC\n";
        std::cout << Eigen.AV(true);
        std::cout << "E*VRC\n";
        std::cout << Eigen.EV(true);
        std::cout << "VLC**H*A\n";
        std::cout << Eigen.AV(false);
        std::cout << "VLC**H*E\n";
        std::cout << Eigen.EV(false);
    }
    for (int k = 0; k <= 1; ++k)
        for (size_t i = 0; i < Eigen.AV(k).GetRowsNb(); i++)
            for (size_t j = 0; j < Eigen.AV(k).GetColsNb(); j++)
            {
                REQUIRE(fabs(Eigen.AV(k)[i][j].real() - Eigen.EV(k)[i][j].real()) < 1e-5);
                REQUIRE(fabs(Eigen.AV(k)[i][j].imag() - Eigen.EV(k)[i][j].imag()) < 1e-5);
            }
}

TEST_CASE("[lapack][EIGEN] double 1")
{
    la::Matrix<double> A{
        {4, 4, -3, 3},
        2, 2
    };
    int flags = 0;
    flags |= la::EIGEN::COMPUTE_VL;
    flags |= la::EIGEN::COMPUTE_VR;
    la::MatrixEigen<double> Eigen{A};
    CHECK_NOTHROW(Eigen.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Eigenvalues and eigenvectors of double matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Eigen.E().GetRowsNb() << " x " << Eigen.E().GetColsNb() << "\n";
        std::cout << Eigen.E();
        std::cout << "EC " << Eigen.EC().GetRowsNb() << " x " << Eigen.EC().GetColsNb() << "\n";
        std::cout << Eigen.EC();
        std::cout << "VL " << Eigen.VL().GetRowsNb() << " x " << Eigen.VL().GetColsNb() << "\n";
        std::cout << Eigen.VL();
        std::cout << "VLC" << Eigen.VLC().GetRowsNb() << " x " << Eigen.VLC().GetColsNb() << "\n";
        std::cout << Eigen.VLC();
        std::cout << "VR " << Eigen.VR().GetRowsNb() << " x " << Eigen.VR().GetColsNb() << "\n";
        std::cout << Eigen.VR();
        std::cout << "VRC" << Eigen.VRC().GetRowsNb() << " x " << Eigen.VRC().GetColsNb() << "\n";
        std::cout << Eigen.VRC();
        std::cout << "A*VRC\n";
        std::cout << Eigen.AV(true);
        std::cout << "E*VRC\n";
        std::cout << Eigen.EV(true);
        std::cout << "VLC**H*A\n";
        std::cout << Eigen.AV(false);
        std::cout << "VLC**H*E\n";
        std::cout << Eigen.EV(false);
    }
    for (int k = 0; k <= 1; ++k)
        for (size_t i = 0; i < Eigen.AV(k).GetRowsNb(); i++)
            for (size_t j = 0; j < Eigen.AV(k).GetColsNb(); j++)
            {
                REQUIRE(fabs(Eigen.AV(k)[i][j].real() - Eigen.EV(k)[i][j].real()) < 1e-5);
                REQUIRE(fabs(Eigen.AV(k)[i][j].imag() - Eigen.EV(k)[i][j].imag()) < 1e-5);
            }
}

TEST_CASE("[lapack][EIGEN] double 2")
{
    la::Matrix<double> A{
        {0, 1, -1, 0},
        2, 2
    };
    int flags = 0;
    flags |= la::EIGEN::COMPUTE_VL;
    flags |= la::EIGEN::COMPUTE_VR;
    la::MatrixEigen<double> Eigen{A};
    CHECK_NOTHROW(Eigen.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Eigenvalues and eigenvectors of double matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Eigen.E().GetRowsNb() << " x " << Eigen.E().GetColsNb() << "\n";
        std::cout << Eigen.E();
        std::cout << "EC " << Eigen.EC().GetRowsNb() << " x " << Eigen.EC().GetColsNb() << "\n";
        std::cout << Eigen.EC();
        std::cout << "VL " << Eigen.VL().GetRowsNb() << " x " << Eigen.VL().GetColsNb() << "\n";
        std::cout << Eigen.VL();
        std::cout << "VLC" << Eigen.VLC().GetRowsNb() << " x " << Eigen.VLC().GetColsNb() << "\n";
        std::cout << Eigen.VLC();
        std::cout << "VR " << Eigen.VR().GetRowsNb() << " x " << Eigen.VR().GetColsNb() << "\n";
        std::cout << Eigen.VR();
        std::cout << "VRC" << Eigen.VRC().GetRowsNb() << " x " << Eigen.VRC().GetColsNb() << "\n";
        std::cout << Eigen.VRC();
        std::cout << "A*VRC\n";
        std::cout << Eigen.AV(true);
        std::cout << "E*VRC\n";
        std::cout << Eigen.EV(true);
        std::cout << "VLC**H*A\n";
        std::cout << Eigen.AV(false);
        std::cout << "VLC**H*E\n";
        std::cout << Eigen.EV(false);
    }
    for (int k = 0; k <= 1; ++k)
        for (size_t i = 0; i < Eigen.AV(k).GetRowsNb(); i++)
            for (size_t j = 0; j < Eigen.AV(k).GetColsNb(); j++)
            {
                REQUIRE(fabs(Eigen.AV(k)[i][j].real() - Eigen.EV(k)[i][j].real()) < 1e-5);
                REQUIRE(fabs(Eigen.AV(k)[i][j].imag() - Eigen.EV(k)[i][j].imag()) < 1e-5);
            }
}

TEST_CASE("[lapack][EIGEN] singular double 3")
{
    la::Matrix<double> A{
        {1, 2, 3, 2, 4, 6, 6, -2, 4},
        3, 3
    };
    int flags = 0;
    flags |= la::EIGEN::COMPUTE_VL;
    flags |= la::EIGEN::COMPUTE_VR;
    la::MatrixEigen<double> Eigen{A};
    CHECK_NOTHROW(Eigen.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Eigenvalues and eigenvectors of double matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Eigen.E().GetRowsNb() << " x " << Eigen.E().GetColsNb() << "\n";
        std::cout << Eigen.E();
        std::cout << "EC " << Eigen.EC().GetRowsNb() << " x " << Eigen.EC().GetColsNb() << "\n";
        std::cout << Eigen.EC();
        std::cout << "VL " << Eigen.VL().GetRowsNb() << " x " << Eigen.VL().GetColsNb() << "\n";
        std::cout << Eigen.VL();
        std::cout << "VLC" << Eigen.VLC().GetRowsNb() << " x " << Eigen.VLC().GetColsNb() << "\n";
        std::cout << Eigen.VLC();
        std::cout << "VR " << Eigen.VR().GetRowsNb() << " x " << Eigen.VR().GetColsNb() << "\n";
        std::cout << Eigen.VR();
        std::cout << "VRC" << Eigen.VRC().GetRowsNb() << " x " << Eigen.VRC().GetColsNb() << "\n";
        std::cout << Eigen.VRC();
        std::cout << "A*VRC\n";
        std::cout << Eigen.AV(true);
        std::cout << "E*VRC\n";
        std::cout << Eigen.EV(true);
        std::cout << "VLC**H*A\n";
        std::cout << Eigen.AV(false);
        std::cout << "VLC**H*E\n";
        std::cout << Eigen.EV(false);
    }
    for (int k = 0; k <= 1; ++k)
        for (size_t i = 0; i < Eigen.AV(k).GetRowsNb(); i++)
            for (size_t j = 0; j < Eigen.AV(k).GetColsNb(); j++)
            {
                REQUIRE(fabs(Eigen.AV(k)[i][j].real() - Eigen.EV(k)[i][j].real()) < 1e-5);
                REQUIRE(fabs(Eigen.AV(k)[i][j].imag() - Eigen.EV(k)[i][j].imag()) < 1e-5);
            }
}

TEST_CASE("[lapack][EIGEN] complex<float> 1")
{
    la::Matrix<std::complex<float>> A{
        {4, 4, -3, 3},
        2, 2
    };
    int flags = 0;
    flags |= la::EIGEN::COMPUTE_VL;
    flags |= la::EIGEN::COMPUTE_VR;
    la::MatrixEigen<std::complex<float>> Eigen{A};
    CHECK_NOTHROW(Eigen.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Eigenvalues and eigenvectors of complex<float> matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Eigen.E().GetRowsNb() << " x " << Eigen.E().GetColsNb() << "\n";
        std::cout << Eigen.E();
        std::cout << "EC " << Eigen.EC().GetRowsNb() << " x " << Eigen.EC().GetColsNb() << "\n";
        std::cout << Eigen.EC();
        std::cout << "VL " << Eigen.VL().GetRowsNb() << " x " << Eigen.VL().GetColsNb() << "\n";
        std::cout << Eigen.VL();
        std::cout << "VLC" << Eigen.VLC().GetRowsNb() << " x " << Eigen.VLC().GetColsNb() << "\n";
        std::cout << Eigen.VLC();
        std::cout << "VR " << Eigen.VR().GetRowsNb() << " x " << Eigen.VR().GetColsNb() << "\n";
        std::cout << Eigen.VR();
        std::cout << "VRC" << Eigen.VRC().GetRowsNb() << " x " << Eigen.VRC().GetColsNb() << "\n";
        std::cout << Eigen.VRC();
        std::cout << "A*VRC\n";
        std::cout << Eigen.AV(true);
        std::cout << "E*VRC\n";
        std::cout << Eigen.EV(true);
        std::cout << "VLC**H*A\n";
        std::cout << Eigen.AV(false);
        std::cout << "VLC**H*E\n";
        std::cout << Eigen.EV(false);
    }
    for (int k = 0; k <= 1; ++k)
        for (size_t i = 0; i < Eigen.AV(k).GetRowsNb(); i++)
            for (size_t j = 0; j < Eigen.AV(k).GetColsNb(); j++)
            {
                REQUIRE(fabs(Eigen.AV(k)[i][j].real() - Eigen.EV(k)[i][j].real()) < 1e-5);
                REQUIRE(fabs(Eigen.AV(k)[i][j].imag() - Eigen.EV(k)[i][j].imag()) < 1e-5);
            }
}

TEST_CASE("[lapack][EIGEN] complex<float> 2")
{
    la::Matrix<std::complex<float>> A{
        {0, 1, -1, 0},
        2, 2
    };
    int flags = 0;
    flags |= la::EIGEN::COMPUTE_VL;
    flags |= la::EIGEN::COMPUTE_VR;
    la::MatrixEigen<std::complex<float>> Eigen{A};
    CHECK_NOTHROW(Eigen.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Eigenvalues and eigenvectors of complex<float> matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Eigen.E().GetRowsNb() << " x " << Eigen.E().GetColsNb() << "\n";
        std::cout << Eigen.E();
        std::cout << "EC " << Eigen.EC().GetRowsNb() << " x " << Eigen.EC().GetColsNb() << "\n";
        std::cout << Eigen.EC();
        std::cout << "VL " << Eigen.VL().GetRowsNb() << " x " << Eigen.VL().GetColsNb() << "\n";
        std::cout << Eigen.VL();
        std::cout << "VLC" << Eigen.VLC().GetRowsNb() << " x " << Eigen.VLC().GetColsNb() << "\n";
        std::cout << Eigen.VLC();
        std::cout << "VR " << Eigen.VR().GetRowsNb() << " x " << Eigen.VR().GetColsNb() << "\n";
        std::cout << Eigen.VR();
        std::cout << "VRC" << Eigen.VRC().GetRowsNb() << " x " << Eigen.VRC().GetColsNb() << "\n";
        std::cout << Eigen.VRC();
        std::cout << "A*VRC\n";
        std::cout << Eigen.AV(true);
        std::cout << "E*VRC\n";
        std::cout << Eigen.EV(true);
        std::cout << "VLC**H*A\n";
        std::cout << Eigen.AV(false);
        std::cout << "VLC**H*E\n";
        std::cout << Eigen.EV(false);
    }
    for (int k = 0; k <= 1; ++k)
        for (size_t i = 0; i < Eigen.AV(k).GetRowsNb(); i++)
            for (size_t j = 0; j < Eigen.AV(k).GetColsNb(); j++)
            {
                REQUIRE(fabs(Eigen.AV(k)[i][j].real() - Eigen.EV(k)[i][j].real()) < 1e-5);
                REQUIRE(fabs(Eigen.AV(k)[i][j].imag() - Eigen.EV(k)[i][j].imag()) < 1e-5);
            }
}

TEST_CASE("[lapack][EIGEN] complex<double> 1")
{
    la::Matrix<std::complex<double>> A{
        {4, 4, -3, 3},
        2, 2
    };
    int flags = 0;
    flags |= la::EIGEN::COMPUTE_VL;
    flags |= la::EIGEN::COMPUTE_VR;
    la::MatrixEigen<std::complex<double>> Eigen{A};
    CHECK_NOTHROW(Eigen.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Eigenvalues and eigenvectors of complex<double> matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Eigen.E().GetRowsNb() << " x " << Eigen.E().GetColsNb() << "\n";
        std::cout << Eigen.E();
        std::cout << "EC " << Eigen.EC().GetRowsNb() << " x " << Eigen.EC().GetColsNb() << "\n";
        std::cout << Eigen.EC();
        std::cout << "VL " << Eigen.VL().GetRowsNb() << " x " << Eigen.VL().GetColsNb() << "\n";
        std::cout << Eigen.VL();
        std::cout << "VLC" << Eigen.VLC().GetRowsNb() << " x " << Eigen.VLC().GetColsNb() << "\n";
        std::cout << Eigen.VLC();
        std::cout << "VR " << Eigen.VR().GetRowsNb() << " x " << Eigen.VR().GetColsNb() << "\n";
        std::cout << Eigen.VR();
        std::cout << "VRC" << Eigen.VRC().GetRowsNb() << " x " << Eigen.VRC().GetColsNb() << "\n";
        std::cout << Eigen.VRC();
        std::cout << "A*VRC\n";
        std::cout << Eigen.AV(true);
        std::cout << "E*VRC\n";
        std::cout << Eigen.EV(true);
        std::cout << "VLC**H*A\n";
        std::cout << Eigen.AV(false);
        std::cout << "VLC**H*E\n";
        std::cout << Eigen.EV(false);
    }
    for (int k = 0; k <= 1; ++k)
        for (size_t i = 0; i < Eigen.AV(k).GetRowsNb(); i++)
            for (size_t j = 0; j < Eigen.AV(k).GetColsNb(); j++)
            {
                REQUIRE(fabs(Eigen.AV(k)[i][j].real() - Eigen.EV(k)[i][j].real()) < 1e-5);
                REQUIRE(fabs(Eigen.AV(k)[i][j].imag() - Eigen.EV(k)[i][j].imag()) < 1e-5);
            }
}

TEST_CASE("[lapack][EIGEN] complex<double> 2")
{
    la::Matrix<std::complex<double>> A{
        {0, 1, -1, 0},
        2, 2
    };
    int flags = 0;
    flags |= la::EIGEN::COMPUTE_VL;
    flags |= la::EIGEN::COMPUTE_VR;
    la::MatrixEigen<std::complex<double>> Eigen{A};
    CHECK_NOTHROW(Eigen.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Eigenvalues and eigenvectors of complex<double> matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Eigen.E().GetRowsNb() << " x " << Eigen.E().GetColsNb() << "\n";
        std::cout << Eigen.E();
        std::cout << "EC " << Eigen.EC().GetRowsNb() << " x " << Eigen.EC().GetColsNb() << "\n";
        std::cout << Eigen.EC();
        std::cout << "VL " << Eigen.VL().GetRowsNb() << " x " << Eigen.VL().GetColsNb() << "\n";
        std::cout << Eigen.VL();
        std::cout << "VLC" << Eigen.VLC().GetRowsNb() << " x " << Eigen.VLC().GetColsNb() << "\n";
        std::cout << Eigen.VLC();
        std::cout << "VR " << Eigen.VR().GetRowsNb() << " x " << Eigen.VR().GetColsNb() << "\n";
        std::cout << Eigen.VR();
        std::cout << "VRC" << Eigen.VRC().GetRowsNb() << " x " << Eigen.VRC().GetColsNb() << "\n";
        std::cout << Eigen.VRC();
        std::cout << "A*VRC\n";
        std::cout << Eigen.AV(true);
        std::cout << "E*VRC\n";
        std::cout << Eigen.EV(true);
        std::cout << "VLC**H*A\n";
        std::cout << Eigen.AV(false);
        std::cout << "VLC**H*E\n";
        std::cout << Eigen.EV(false);
    }
    for (int k = 0; k <= 1; ++k)
        for (size_t i = 0; i < Eigen.AV(k).GetRowsNb(); i++)
            for (size_t j = 0; j < Eigen.AV(k).GetColsNb(); j++)
            {
                REQUIRE(fabs(Eigen.AV(k)[i][j].real() - Eigen.EV(k)[i][j].real()) < 1e-5);
                REQUIRE(fabs(Eigen.AV(k)[i][j].imag() - Eigen.EV(k)[i][j].imag()) < 1e-5);
            }
}

TEST_CASE("[lapack][EIGEN] singular complex<double> 3")
{
    la::Matrix<std::complex<double>> A{
        {1, 2, 3, 2, 4, 6, 6, -2, 4},
        3, 3
    };
    int flags = 0;
    flags |= la::EIGEN::COMPUTE_VL;
    flags |= la::EIGEN::COMPUTE_VR;
    la::MatrixEigen<std::complex<double>> Eigen{A};
    CHECK_NOTHROW(Eigen.Compute(flags));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "Eigenvalues and eigenvectors of complex<double> matrix \n";
        std::cout << "A " << A.GetRowsNb() << " x " << A.GetColsNb() << "\n";
        std::cout << A;
        std::cout << "E " << Eigen.E().GetRowsNb() << " x " << Eigen.E().GetColsNb() << "\n";
        std::cout << Eigen.E();
        std::cout << "EC " << Eigen.EC().GetRowsNb() << " x " << Eigen.EC().GetColsNb() << "\n";
        std::cout << Eigen.EC();
        std::cout << "VL " << Eigen.VL().GetRowsNb() << " x " << Eigen.VL().GetColsNb() << "\n";
        std::cout << Eigen.VL();
        std::cout << "VLC" << Eigen.VLC().GetRowsNb() << " x " << Eigen.VLC().GetColsNb() << "\n";
        std::cout << Eigen.VLC();
        std::cout << "VR " << Eigen.VR().GetRowsNb() << " x " << Eigen.VR().GetColsNb() << "\n";
        std::cout << Eigen.VR();
        std::cout << "VRC" << Eigen.VRC().GetRowsNb() << " x " << Eigen.VRC().GetColsNb() << "\n";
        std::cout << Eigen.VRC();
        std::cout << "A*VRC\n";
        std::cout << Eigen.AV(true);
        std::cout << "E*VRC\n";
        std::cout << Eigen.EV(true);
        std::cout << "VLC**H*A\n";
        std::cout << Eigen.AV(false);
        std::cout << "VLC**H*E\n";
        std::cout << Eigen.EV(false);
    }
    for (int k = 0; k <= 1; ++k)
        for (size_t i = 0; i < Eigen.AV(k).GetRowsNb(); i++)
            for (size_t j = 0; j < Eigen.AV(k).GetColsNb(); j++)
            {
                REQUIRE(fabs(Eigen.AV(k)[i][j].real() - Eigen.EV(k)[i][j].real()) < 1e-5);
                REQUIRE(fabs(Eigen.AV(k)[i][j].imag() - Eigen.EV(k)[i][j].imag()) < 1e-5);
            }
}

TEST_SUITE_END();

#endif
