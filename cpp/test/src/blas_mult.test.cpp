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
#include "algebra/matrix.h"
#include "algebra/matrix_operations.h"
#include "std/cout.h"
#include "doctest.h"
#include "global_static.h"

TEST_SUITE_BEGIN("[BLAS]");

TEST_CASE("[BLAS] float matrix to vector product")
{
    la::Matrix<float> m1{
        std::vector<float>{4.5f, 2.0f, 0.5f, 3.2f, 0.0f, 7.4f, 5.6f, 3.2f, 0.0f, 1.1f, 1.3f, 7.1f},
        3, 4
    };
    la::Matrix<float> v1{
        std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f}
    };
    la::Matrix<float> v2 = la::MatMultVec(m1, v1);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "matrix multiplication of m1 and m2\n";
        std::cout << m1 << v1 << v2;
    }
    REQUIRE(fabs(v2[0][0] - 22.8) < 1e-5);
    REQUIRE(fabs(v2[1][0] - 44.4) < 1e-5);
    REQUIRE(fabs(v2[2][0] - 34.5) < 1e-5);
}

TEST_CASE("[BLAS] double matrix to vector product")
{
    la::Matrix<double> m1{
        std::vector<double>{4.5, 2.0, 0.5, 3.2, 0.0, 7.4, 5.6, 3.2, 0.0, 1.1, 1.3, 7.1},
        3, 4
    };
    la::Matrix<double> v1{
        std::vector<double>{1.0, 2.0, 3.0, 4.0}
    };
    la::Matrix<double> v2 = la::MatMultVec(m1, v1);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "matrix multiplication of m1 and m2\n";
        std::cout << m1 << v1 << v2;
    }
    REQUIRE(fabs(v2[0][0] - 22.8) < 1e-5);
    REQUIRE(fabs(v2[1][0] - 44.4) < 1e-5);
    REQUIRE(fabs(v2[2][0] - 34.5) < 1e-5);
}

TEST_CASE("[BLAS] complex float matrix to vector product 1")
{
    // clang-format off
    la::Matrix<std::complex<float>> m1{
        std::vector<std::complex<float>>{{4.5f, 0.f}, {2.0f, 0.f}, {0.5f, 0.f}, {3.2f, 0.f}, {0.0f, 0.f},
            {7.4f, 0.f}, {5.6f, 0.f}, {3.2f, 0.f}, {0.0f, 0.f}, {1.1f, 0.f}, {1.3f, 0.f}, {7.1f, 0.f}},
            3, 4
    };
    // clang-format on
    la::Matrix<std::complex<float>> v1{
        std::vector<std::complex<float>>{{1, 0}, {2, 0}, {3, 0}, {4, 0}}
    };
    la::Matrix<std::complex<float>> v2 = la::MatMultVec(m1, v1);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "matrix multiplication of m1 and m2\n";
        std::cout << m1 << v1 << v2;
    }
    REQUIRE(fabs(v2[0][0].real() - 22.8) < 1e-5);
    REQUIRE(fabs(v2[0][0].imag() - 0.0) < 1e-5);
    REQUIRE(fabs(v2[1][0].real() - 44.4) < 1e-5);
    REQUIRE(fabs(v2[1][0].imag() - 0.0) < 1e-5);
    REQUIRE(fabs(v2[2][0].real() - 34.5) < 1e-5);
    REQUIRE(fabs(v2[2][0].imag() - 0.0) < 1e-5);
}

TEST_CASE("[BLAS] complex float matrix to vector product 2")
{
    // clang-format off
    la::Matrix<std::complex<float>> m1{
        std::vector<std::complex<float>>{ {1.2f, 2.3f}, {-2.4f, 3.5f}, {3.6f, -4.7f}, {-4.8f, -5.9f},
            {-5.1f, 6.2f}, {6.3f, -7.4f}, {-7.5f, 8.6f}, {8.7f, -9.8f},
            {9.9f, -10.0f}, {-10.1f, 11.2f}, {11.3f, -12.4f}, {-12.5f, -13.6f},
            {-13.7f, 14.8f}, {14.9f, -15.0f}, {-15.1f, 16.2f}, {16.3f, -17.4f}},
            4, 4
    };
    // clang-format on
    la::Matrix<std::complex<float>> v1{
        std::vector<std::complex<float>>{{-1.5f, 2.6f}, {2.7f, -3.8f}, {-3.9f, 4.0f}, {4.1f, -5.2f}}
    };
    la::Matrix<std::complex<float>> v2 = la::MatMultVec(m1, v1);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "matrix multiplication of m1 and m2\n";
        std::cout << m1 << v1 << v2;
    }
    REQUIRE(fabs(v2[0][0].real() + 46.56) < 1e-4);
    REQUIRE(fabs(v2[0][0].imag() - 51.74) < 1e-4);
    REQUIRE(fabs(v2[1][0].real() + 40.02) < 1e-4);
    REQUIRE(fabs(v2[1][0].imag() + 215.44) < 1e-4);
    REQUIRE(fabs(v2[2][0].real() + 90.0) < 1e-4);
    REQUIRE(fabs(v2[2][0].imag() - 212.16) < 1e-4);
    REQUIRE(fabs(v2[3][0].real() + 64.26) < 1e-4);
    REQUIRE(fabs(v2[3][0].imag() + 434.62) < 1e-4);
}

TEST_CASE("[BLAS] complex double matrix to vector product 1")
{
    // clang-format off
    la::Matrix<std::complex<double>> m1{
        std::vector<std::complex<double>>{{4.5, 0}, {2.0, 0}, {0.5, 0}, {3.2, 0}, {0.0, 0},
            {7.4, 0}, {5.6, 0}, {3.2, 0}, {0.0, 0}, {1.1, 0}, {1.3, 0}, {7.1, 0}},
            3, 4
    };
    // clang-format on
    la::Matrix<std::complex<double>> v1{
        std::vector<std::complex<double>>{{1, 0}, {2, 0}, {3, 0}, {4, 0}}
    };
    la::Matrix<std::complex<double>> v2 = la::MatMultVec(m1, v1);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "matrix multiplication of m1 and m2\n";
        std::cout << m1 << v1 << v2;
    }
    REQUIRE(fabs(v2[0][0].real() - 22.8) < 1e-5);
    REQUIRE(fabs(v2[0][0].imag() - 0.0) < 1e-5);
    REQUIRE(fabs(v2[1][0].real() - 44.4) < 1e-5);
    REQUIRE(fabs(v2[1][0].imag() - 0.0) < 1e-5);
    REQUIRE(fabs(v2[2][0].real() - 34.5) < 1e-5);
    REQUIRE(fabs(v2[2][0].imag() - 0.0) < 1e-5);
}

TEST_CASE("[BLAS] complex double matrix to vector product 2")
{
    // clang-format off
    la::Matrix<std::complex<double>> m1{
        std::vector<std::complex<double>>{ {1.2, 2.3}, {-2.4, 3.5}, {3.6, -4.7}, {-4.8, -5.9},
            {-5.1, 6.2}, {6.3, -7.4}, {-7.5, 8.6}, {8.7, -9.8},
            {9.9, -10.0}, {-10.1, 11.2}, {11.3, -12.4}, {-12.5, -13.6},
            {-13.7, 14.8}, {14.9, -15.0}, {-15.1, 16.2}, {16.3, -17.4}},
            4, 4
    };
    // clang-format on
    la::Matrix<std::complex<double>> v1{
        std::vector<std::complex<double>>{{-1.5, 2.6}, {2.7, -3.8}, {-3.9, 4.0}, {4.1, -5.2}}
    };
    la::Matrix<std::complex<double>> v2 = la::MatMultVec(m1, v1);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "matrix multiplication of m1 and m2\n";
        std::cout << m1 << v1 << v2;
    }
    REQUIRE(fabs(v2[0][0].real() + 46.56) < 1e-5);
    REQUIRE(fabs(v2[0][0].imag() - 51.74) < 1e-5);
    REQUIRE(fabs(v2[1][0].real() + 40.02) < 1e-5);
    REQUIRE(fabs(v2[1][0].imag() + 215.44) < 1e-5);
    REQUIRE(fabs(v2[2][0].real() + 90.0) < 1e-5);
    REQUIRE(fabs(v2[2][0].imag() - 212.16) < 1e-5);
    REQUIRE(fabs(v2[3][0].real() + 64.26) < 1e-5);
    REQUIRE(fabs(v2[3][0].imag() + 434.62) < 1e-5);
}

TEST_CASE("[BLAS] float matrix to matrix product")
{
    la::Matrix<float> m1{
        std::vector<float>{4.5f, 2.0f, 0.5f, 3.2f, 0.0f, 7.4f, 5.6f, 3.2f, 0.0f, 1.1f, 1.3f, 7.1f},
        3, 4
    };
    la::Matrix<float> m2{
        std::vector<float>{2.5f, 3.2f, 0.7f, 2.1f, 0.5f, 1.4f, 4.1f, 3.4f},
        4, 2
    };
    la::Matrix<float> m3 = la::MatMult(m1, m2);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "matrix multiplication of m1 and m2\n";
        std::cout << m1 << m2 << m3;
    }
    REQUIRE(fabs(m3[0][0] - 26.02) < 1e-5);
    REQUIRE(fabs(m3[0][1] - 30.18) < 1e-5);
    REQUIRE(fabs(m3[1][0] - 21.1) < 1e-5);
    REQUIRE(fabs(m3[1][1] - 34.26) < 1e-5);
    REQUIRE(fabs(m3[2][0] - 30.53) < 1e-5);
    REQUIRE(fabs(m3[2][1] - 28.27) < 1e-5);
}

TEST_CASE("[BLAS] double matrix to matrix product")
{
    la::Matrix<double> m1{
        std::vector<double>{4.5, 2.0, 0.5, 3.2, 0.0, 7.4, 5.6, 3.2, 0.0, 1.1, 1.3, 7.1},
        3, 4
    };
    la::Matrix<double> m2{
        std::vector<double>{2.5, 3.2, 0.7, 2.1, 0.5, 1.4, 4.1, 3.4},
        4, 2
    };
    la::Matrix<double> m3 = la::MatMult(m1, m2);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "matrix multiplication of m1 and m2\n";
        std::cout << m1 << m2 << m3;
    }
    REQUIRE(fabs(m3[0][0] - 26.02) < 1e-5);
    REQUIRE(fabs(m3[0][1] - 30.18) < 1e-5);
    REQUIRE(fabs(m3[1][0] - 21.1) < 1e-5);
    REQUIRE(fabs(m3[1][1] - 34.26) < 1e-5);
    REQUIRE(fabs(m3[2][0] - 30.53) < 1e-5);
    REQUIRE(fabs(m3[2][1] - 28.27) < 1e-5);
}

TEST_CASE("[BLAS] complex float matrix to matrix product 1")
{
    // clang-format off
    la::Matrix<std::complex<float>> m1{
        std::vector<std::complex<float>>{{4.5f, 0.f}, {2.0f, 0.f}, {0.5f, 0.f},  {3.2f, 0.f},
            {0.0f, 0.f}, {7.4f, 0.f}, {5.6f, 0.f}, {3.2f, 0.f},
            {0.0f, 0.f}, {1.1f, 0.f}, {1.3f, 0.f}, {7.1f, 0.f}},
            3, 4
    };
    la::Matrix<std::complex<float>> m2{
        std::vector<std::complex<float>>{{2.5f, 0.f}, {3.2f, 0.f}, {0.7f, 0.f}, {2.1f, 0.f},
            {0.5f, 0.f}, {1.4f, 0.f}, {4.1f, 0.f}, {3.4f, 0.f}},
            4, 2
    };
    // clang-format on
    la::Matrix<std::complex<float>> m3  = la::MatMult(m1, m2);
    la::Matrix<std::complex<float>> m1t = la::MatTranspose(m1), m2t = la::MatTranspose(m2), m3t = la::MatMult(m2t, m1t);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "matrix multiplication of m1 and m2\n";
        std::cout << m1 << m2 << m3;
        std::cout << "matrix multiplication of m2t and m1t\n";
        std::cout << m2t << m1t << m3t;
    }
    REQUIRE(fabs(m3[1][0].real() - 21.1) < 1e-5);
    REQUIRE(fabs(m3[1][0].imag()) < 1e-5);
    REQUIRE(fabs(m3[1][1].real() - 34.26) < 1e-5);
    REQUIRE(fabs(m3[1][1].imag()) < 1e-5);
    REQUIRE(fabs(m3[2][0].real() - 30.53) < 1e-5);
    REQUIRE(fabs(m3[2][0].imag()) < 1e-5);
    REQUIRE(fabs(m3[2][1].real() - 28.27) < 1e-5);
    REQUIRE(fabs(m3[2][1].imag()) < 1e-5);
    for (size_t i = 0; i < m3.GetRowsNb(); ++i)
    {
        for (size_t j = 0; j < m3.GetColsNb(); ++j)
        {
            REQUIRE(fabs(m3[i][j].real() - m3t[j][i].real()) < 1e-7);
            REQUIRE(fabs(m3[i][j].imag() - m3t[j][i].imag()) < 1e-7);
        }
    }
}

TEST_CASE("[BLAS] complex float matrix to matrix product 2")
{
    // clang-format off
    la::Matrix<std::complex<float>> m1{
        std::vector<std::complex<float>>{ {1.2f, 2.3f}, {-2.4f, 3.5f}, {3.6f, -4.7f}, {-4.8f, -5.9f},
            {-5.1f, 6.2f}, {6.3f, -7.4f}, {-7.5f, 8.6f}, {8.7f, -9.8f},
            {9.9f, -10.0f}, {-10.1f, 11.2f}, {11.3f, -12.4f}, {-12.5f, -13.6f},
            {-13.7f, 14.8f}, {14.9f, -15.0f}, {-15.1f, 16.2f}, {16.3f, -17.4f}},
            4, 4
    };
    la::Matrix<std::complex<float>> m2{
        std::vector<std::complex<float>>{{-1.5f, 2.6f}, {-3.0f, 5.2f},
            {2.7f, -3.8f},{5.4f, -7.6f},
            {-3.9f, 4.0f}, {-7.8f, 8.0f},
            {4.1f, -5.2f}, {8.2f, -10.4f}},
            4, 2
    };
    // clang-format on
    la::Matrix<std::complex<float>> m3  = la::MatMult(m1, m2);
    la::Matrix<std::complex<float>> m1t = la::MatTranspose(m1), m2t = la::MatTranspose(m2), m3t = la::MatMult(m2t, m1t);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "matrix multiplication of m1 and m2\n";
        std::cout << m1 << m2 << m3;
        std::cout << "matrix multiplication of m2t and m1t\n";
        std::cout << m2t << m1t << m3t;
    }
    REQUIRE(fabs(m3[0][0].real() + 46.56) < 1e-4);
    REQUIRE(fabs(m3[0][0].imag() - 51.74) < 1e-4);
    REQUIRE(fabs(m3[1][0].real() + 40.02) < 1e-4);
    REQUIRE(fabs(m3[1][0].imag() + 215.44) < 1e-4);
    REQUIRE(fabs(m3[2][0].real() + 90.0) < 1e-4);
    REQUIRE(fabs(m3[2][0].imag() - 212.16) < 1e-4);
    REQUIRE(fabs(m3[3][0].real() + 64.26) < 1e-4);
    REQUIRE(fabs(m3[3][0].imag() + 434.62) < 1e-4);
    for (size_t i = 0; i < m3.GetRowsNb(); ++i)
    {
        REQUIRE(fabs(m3[i][1].real() - 2 * m3[i][0].real()) < 1e-4);
        REQUIRE(fabs(m3[i][1].imag() - 2 * m3[i][0].imag()) < 1e-4);
    }
    for (size_t i = 0; i < m3.GetRowsNb(); ++i)
    {
        for (size_t j = 0; j < m3.GetColsNb(); ++j)
        {
            REQUIRE(fabs(m3[i][j].real() - m3t[j][i].real()) < 1e-7);
            REQUIRE(fabs(m3[i][j].imag() - m3t[j][i].imag()) < 1e-7);
        }
    }
}

TEST_CASE("[BLAS] complex double matrix to matrix product 1")
{
    // clang-format off
    la::Matrix<std::complex<double>> m1{
        std::vector<std::complex<double>>{{4.5, 0.0}, {2.0, 0.0}, {0.5, 0.0}, {3.2, 0.0},
            {0.0, 0.0}, {7.4, 0.0}, {5.6, 0.0}, {3.2, 0.0},
            {0.0, 0.0}, {1.1, 0.0}, {1.3, 0.0}, {7.1, 0.0}},
            3, 4
    };
    la::Matrix<std::complex<double>> m2{
        std::vector<std::complex<double>>{{2.5, 0.0}, {3.2, 0.0}, {0.7, 0.0}, {2.1, 0.0}, {0.5, 0.0},
            {1.4, 0.0}, {4.1, 0.0}, {3.4, 0.0}},
            4, 2
    };
    // clang-format on
    la::Matrix<std::complex<double>> m3  = la::MatMult(m1, m2);
    la::Matrix<std::complex<double>> m1t = la::MatTranspose(m1), m2t = la::MatTranspose(m2),
                                     m3t = la::MatMult(m2t, m1t);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "matrix multiplication of m1 and m2\n";
        std::cout << m1 << m2 << m3;
        std::cout << "matrix multiplication of m2t and m1t\n";
        std::cout << m2t << m1t << m3t;
    }
    REQUIRE(fabs(m3[1][0].real() - 21.1) < 1e-5);
    REQUIRE(fabs(m3[1][0].imag()) < 1e-5);
    REQUIRE(fabs(m3[1][1].real() - 34.26) < 1e-5);
    REQUIRE(fabs(m3[1][1].imag()) < 1e-5);
    REQUIRE(fabs(m3[2][0].real() - 30.53) < 1e-5);
    REQUIRE(fabs(m3[2][0].imag()) < 1e-5);
    REQUIRE(fabs(m3[2][1].real() - 28.27) < 1e-5);
    REQUIRE(fabs(m3[2][1].imag()) < 1e-5);
    for (size_t i = 0; i < m3.GetRowsNb(); ++i)
    {
        for (size_t j = 0; j < m3.GetColsNb(); ++j)
        {
            REQUIRE(fabs(m3[i][j].real() - m3t[j][i].real()) < 1e-7);
            REQUIRE(fabs(m3[i][j].imag() - m3t[j][i].imag()) < 1e-7);
        }
    }
}

TEST_CASE("[BLAS] complex double matrix to matrix product 2")
{
    // clang-format off
    la::Matrix<std::complex<double>> m1{
        std::vector<std::complex<double>>{{1.2, 2.3}, {-2.4, 3.5}, {3.6, -4.7}, {-4.8, -5.9},
            {-5.1, 6.2}, {6.3, -7.4}, {-7.5, 8.6}, {8.7, -9.8},
            {9.9, -10.0}, {-10.1, 11.2}, {11.3, -12.4}, {-12.5, -13.6},
            {-13.7, 14.8}, {14.9, -15.0}, {-15.1, 16.2}, {16.3, -17.4}},
            4, 4
    };
    la::Matrix<std::complex<double>> m2{
        std::vector<std::complex<double>>{{-1.5, 2.6}, {-3.0, 5.2},
            {2.7, -3.8},{5.4, -7.6},
            {-3.9, 4.0}, {-7.8, 8.0},
            {4.1, -5.2}, {8.2, -10.4}},
            4, 2
    };
    // clang-format on
    la::Matrix<std::complex<double>> m3  = la::MatMult(m1, m2);
    la::Matrix<std::complex<double>> m1t = la::MatTranspose(m1), m2t = la::MatTranspose(m2),
                                     m3t = la::MatMult(m2t, m1t);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "matrix multiplication of m1 and m2\n";
        std::cout << m1 << m2 << m3;
        std::cout << "matrix multiplication of m2t and m1t\n";
        std::cout << m2t << m1t << m3t;
    }
    REQUIRE(fabs(m3[0][0].real() + 46.56) < 1e-4);
    REQUIRE(fabs(m3[0][0].imag() - 51.74) < 1e-4);
    REQUIRE(fabs(m3[1][0].real() + 40.02) < 1e-4);
    REQUIRE(fabs(m3[1][0].imag() + 215.44) < 1e-4);
    REQUIRE(fabs(m3[2][0].real() + 90.0) < 1e-4);
    REQUIRE(fabs(m3[2][0].imag() - 212.16) < 1e-4);
    REQUIRE(fabs(m3[3][0].real() + 64.26) < 1e-4);
    REQUIRE(fabs(m3[3][0].imag() + 434.62) < 1e-4);
    for (size_t i = 0; i < m3.GetRowsNb(); ++i)
    {
        REQUIRE(fabs(m3[i][1].real() - 2 * m3[i][0].real()) < 1e-7);
        REQUIRE(fabs(m3[i][1].imag() - 2 * m3[i][0].imag()) < 1e-7);
    }
    for (size_t i = 0; i < m3.GetRowsNb(); ++i)
    {
        for (size_t j = 0; j < m3.GetColsNb(); ++j)
        {
            REQUIRE(fabs(m3[i][j].real() - m3t[j][i].real()) < 1e-7);
            REQUIRE(fabs(m3[i][j].imag() - m3t[j][i].imag()) < 1e-7);
        }
    }
}

TEST_SUITE_END();

#endif
