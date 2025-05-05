#ifndef __clang_analyzer__
/**************************/
/*   matrix.test.cpp      */
/*    Version 1.0         */
/*     2023/01/29         */
/**************************/

#include <array>
#include "algebra/matrix.h"
#include "algebra/matrix_operations.h"
#include "std/cout.h"
#include "doctest.h"
#include "global_static.h"
#include "la_decomposition.h"
#include "la_utils.h"

TEST_SUITE_BEGIN("[matrix]");

TEST_CASE("[matrix] row")
{
    la::Matrix<double> m1{
        std::vector<double>{4.5, 2.0, 0.5, 3.2, 0.0, 7.4, 5.6, 3.2, 0.0, 1.1, 1.3, 7.1},
        3, 4
    };
    la::Matrix<double> m2{
        std::vector<double>{2.5, 3.2, 0.7, 2.1, 0.5, 1.4, 4.1, 3.4},
        4, 2
    };
    la::Matrix<double> v1{
        std::vector<double>{1.0, 2.0, 3.0, 4.0}
    };
    la::Matrix<double> v2{
        std::vector<double>{5.0, 6.0, 7.0, 8.0}
    };
    la::Matrix<double> v3{
        std::vector<double>{9.0, 10.0, 11.0, 12.0, 13.0}
    };

    if (GLOBAL_VERBOSE_FLAG) std::cout << "matrix and vector operations output";
    // compute the matrix and vector operations
    la::Matrix<double> m3 = la::MatOuter(v1, v3), m4 = la::MatMult(m1, m2);
    double v4 = la::MatDot(v1, v2);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "matrix multiplication of m1 and m2\n";
        std::cout << m1 << m2 << m4;
        std::cout << "outer product of v1 and v3\n";
        std::cout << la::MatTranspose(v1) << la::MatTranspose(v3) << m3;
        std::cout << "dot product of v1 and v2\n";
        std::cout << la::MatTranspose(v1) << la::MatTranspose(v2) << v4 << "\n";
    }
    // matrix multiplication
    REQUIRE(fabs(m4[1][0] - 21.1) < 1e-9);
    REQUIRE(fabs(m4[1][1] - 34.26) < 1e-9);
    REQUIRE(fabs(m4[2][1] - 28.27) < 1e-9);
    // dot product
    REQUIRE(fabs(v4 - 70.0) < 1e-9);
    // outer product
    REQUIRE(fabs(m3[2][0] - 27.0) < 1e-9);
    REQUIRE(fabs(m3[2][3] - 36.0) < 1e-9);

    REQUIRE(m1.GetColsNb() == m2.GetRowsNb());
    REQUIRE(m1.GetColsNb() == v2.GetRowsNb());
}

TEST_CASE("[matrix] insert row")
{
    la::Matrix<float> m1{
        std::vector<float>{4.5f, 2.0f, 0.5f, 3.2f, 0.0f, 7.4f, 5.6f, 3.2f, 0.0f, 1.1f, 1.3f, 7.1f},
        3, 4
    };
    std::vector<float> v1{1.3f, 1.0f, -3.2f, 4.1f};
    if (GLOBAL_VERBOSE_FLAG) std::cout << "insert row\n" << m1;
    m1.InsertRow(1);
    REQUIRE(fabs(m1[1][2]) < 1e-6);
    REQUIRE(fabs(m1[2][2] - 5.6) < 1e-6);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    m1.InsertRow(0, v1);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[0][2] + 3.2) < 1e-6);
    REQUIRE(fabs(m1[2][2]) < 1e-6);
    REQUIRE(fabs(m1[3][2] - 5.6) < 1e-6);
    m1.InsertRow(m1.GetRowsNb(), v1);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[5][0] - 1.3) < 1e-6);
    REQUIRE(fabs(m1[5][2] + 3.2) < 1e-6);
}

TEST_CASE("[matrix] erase row")
{
    la::Matrix<float> m1{
        std::vector<float>{4.5f, 2.0f, 0.5f, 3.2f, 0.0f, 7.4f, 5.6f, 3.2f, 0.0f, 1.1f, 1.3f, 7.1f, 4.5f, -1.2f, 5.6f,
                           6.7F},
        4, 4
    };
    if (GLOBAL_VERBOSE_FLAG) std::cout << "erase row\n" << m1;
    m1.EraseRow(1);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[1][1] - 1.1) < 1e-6);
    REQUIRE(fabs(m1[1][2] - 1.3) < 1e-6);
    m1.EraseRow(0);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[0][0]) < 1e-6);
    REQUIRE(fabs(m1[0][3] - 7.1) < 1e-6);
    REQUIRE(fabs(m1[1][1] + 1.2) < 1e-6);
    m1.EraseRow(m1.GetRowsNb() - 1);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[0][1] - 1.1) < 1e-6);
    REQUIRE(fabs(m1[0][2] - 1.3) < 1e-6);
}

TEST_CASE("[matrix] insert column")
{
    la::Matrix<float> m1{
        std::vector<float>{4.5f, 2.0f, 0.5f, 3.2f, 0.0f, 7.4f, 5.6f, 3.2f, 0.0f, 1.1f, 1.3f, 7.1f},
        3, 4
    };
    std::vector<float> v1{1.3f, 1.0f, -3.2f};
    if (GLOBAL_VERBOSE_FLAG) std::cout << "insert column\n" << m1;
    m1.InsertCol(1);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[0][1]) < 1e-6);
    REQUIRE(fabs(m1[1][3] - 5.6) < 1e-6);
    m1.InsertCol(m1.GetColsNb(), v1);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[0][5] - 1.3) < 1e-6);
    REQUIRE(fabs(m1[1][3] - 5.6) < 1e-6);
    REQUIRE(fabs(m1[2][1]) < 1e-6);
    m1.InsertCol(0, v1);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[0][0] - 1.3) < 1e-6);
    REQUIRE(fabs(m1[2][0] + 3.2) < 1e-6);
    REQUIRE(fabs(m1[1][3] - 7.4) < 1e-6);
    REQUIRE(fabs(m1[2][5] - 7.1) < 1e-6);
}

TEST_CASE("[matrix] erase column")
{
    la::Matrix<float> m1{
        std::vector<float>{4.5f, 2.0f, 0.5f, 3.2f, 0.0f, 7.4f, 5.6f, 3.2f, 0.0f, 1.1f, 1.3f, 7.1f, 4.5f, -1.2f, 5.6f},
        3, 5
    };
    if (GLOBAL_VERBOSE_FLAG) std::cout << "erase column\n" << m1;
    m1.EraseCol(1);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[1][1] - 3.2) < 1e-6);
    REQUIRE(fabs(m1[2][0] - 1.3) < 1e-6);
    m1.EraseCol(0);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[1][1]) < 1e-6);
    REQUIRE(fabs(m1[2][1] + 1.2) < 1e-6);
    REQUIRE(fabs(m1[2][2] - 5.6) < 1e-6);
    m1.EraseCol(m1.GetColsNb() - 1);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[1][1]) < 1e-6);
    REQUIRE(fabs(m1[2][1] + 1.2) < 1e-6);
}

TEST_CASE("[matrix] assign row")
{
    la::Matrix<float> m1{
        std::vector<float>{4.5f, 2.0f, 0.5f, 3.2f, 0.0f, 7.4f, 5.6f, 3.2f, 0.0f, 1.1f, 1.3f, 7.1f},
        3, 4
    },
        m2{1, 4}, m3{4, 2};
    std::vector<float> v1{1.3f, 1.0f, -3.2f, 4.1f};
    if (GLOBAL_VERBOSE_FLAG) std::cout << "assign row\n" << m1;
    m1.assignRow(2, v1);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[2][0] - 1.3) < 1e-6);
    REQUIRE(fabs(m1[2][3] - 4.1) < 1e-6);
    m2.Ones();
    m2 *= 3.0;
    m1.assignRow(1, m2, 0);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[1][0] - 3.0) < 1e-6);
    REQUIRE(fabs(m1[1][3] - 3.0) < 1e-6);
    m3.Ones();
    m3 *= 5.0;
    m1.assignRowCol(1, m3, 1);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[1][0] - 5.0) < 1e-6);
    REQUIRE(fabs(m1[1][3] - 5.0) < 1e-6);
}

TEST_CASE("[matrix] assign column")
{
    la::Matrix<float> m1{
        std::vector<float>{4.5f, 2.0f, 0.5f, 3.2f, 0.0f, 7.4f, 5.6f, 3.2f, 0.0f, 1.1f, 1.3f, 7.1f},
        3, 4
    },
        m2{3, 1}, m3{5, 3};
    std::vector<float> v1{1.3f, 1.0f, 4.1f};
    if (GLOBAL_VERBOSE_FLAG) std::cout << "assign column\n" << m1;
    m1.assignCol(2, v1);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[0][2] - 1.3) < 1e-6);
    REQUIRE(fabs(m1[2][2] - 4.1) < 1e-6);
    m2.Ones();
    m2 *= 3.0;
    m1.assignCol(2, m2, 0);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[0][2] - 3.0) < 1e-6);
    REQUIRE(fabs(m1[1][2] - 3.0) < 1e-6);
    m3.Ones();
    m3 *= 2.5;
    m1.assignColRow(1, m3, 4);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[0][1] - 2.5) < 1e-6);
    REQUIRE(fabs(m1[1][1] - 2.5) < 1e-6);
}

TEST_CASE("[matrix] swap rows")
{
    la::Matrix<double> m1{
        std::vector<double>{4.5, 2.0, 0.5, 3.2, 0.0, 7.4, 5.6, 3.2, 0.0, 1.1, 1.3, 7.1},
        3, 4
    };
    if (GLOBAL_VERBOSE_FLAG) std::cout << "swap rows\n" << m1;
    m1.SwapRows(1, 0);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[0][1] - 7.4) < 1e-9);
    m1.SwapRows(0, 1);
    REQUIRE(fabs(m1[1][1] - 7.4) < 1e-9);
}

TEST_CASE("[matrix] swap columns")
{
    la::Matrix<double> m1{
        std::vector<double>{4.5, 2.0, 0.5, 3.2, 0.0, 7.4, 5.6, 3.2, 0.0, 1.1, 1.3, 7.1},
        3, 4
    };
    if (GLOBAL_VERBOSE_FLAG) std::cout << "swap columns\n" << m1;
    m1.SwapCols(1, 0);
    if (GLOBAL_VERBOSE_FLAG) std::cout << m1;
    REQUIRE(fabs(m1[0][0] - 2.0) < 1e-9);
    REQUIRE(fabs(m1[0][1] - 4.5) < 1e-9);
    m1.SwapCols(0, 1);
    REQUIRE(fabs(m1[1][1] - 7.4) < 1e-9);
}

TEST_CASE("[matrix] reduced row echelon form")
{
    la::Matrix<double> m1{
        std::vector<double>{4.5, 2.0, 0.5, 3.2, 0.0, 7.4, 5.6, 3.2, 0.0, 1.1, 1.3, 7.1},
        3, 4
    };
    la::Matrix<double> m2{
        std::vector<double>{1, 2, 2, 2, 0, 0, 2, 4, 0, 0, 2, 4},
        3, 4
    };
    la::Matrix<double> m3{
        std::vector<double>{1, 3, 2, 1, 6, 1, 5, 1},
        4, 2
    };
    la::Matrix<double> m4{
        std::vector<double>{1, 3, 2, 1, 6, 1, 5, 1, 9},
        3, 3
    };
    la::Matrix<float> m5{
        std::vector<float>{1, 2, 3, 1, 1, 1, 2, 1, 1, 2, 3, 1},
        3, 4
    };

    la::Matrix<double> m1e{3, 4}, m2e{3, 4}, m3e{4, 2}, m4e{3, 3};
    la::Matrix<float> m5e{3, 4};
    REQUIRE(MatRank(m1) == 3);
    REQUIRE(MatRank(m2) == 2);
    REQUIRE(MatRank(m3) == 2);
    REQUIRE(MatRank(m4) == 3);
    REQUIRE(MatRank(m5) == 2);
    la::MatRref(m1e, m1);
    la::MatRref(m2e, m2);
    la::MatRref(m3e, m3);
    la::MatRref(m4e, m4);
    la::MatRref(m5e, m5);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "reduced row echelon form\n";
        std::cout << "M1  [3 x 4]\n";
        std::cout << m1;
        std::cout << "M1e [3 x 4]\n";
        std::cout << m1e;
        std::cout << "M2  [3 x 4]\n";
        std::cout << m2;
        std::cout << "M2e [3 x 4]\n";
        std::cout << m2e;
        std::cout << "M3  [4 x 2]\n";
        std::cout << m3;
        std::cout << "M3e [4 x 2]\n";
        std::cout << m3e;
        std::cout << "M4  [3 x 3]\n";
        std::cout << m4;
        std::cout << "M4e [3 x 3]\n";
        std::cout << m4e;
        std::cout << "M5  [3 x 4]\n";
        std::cout << m5;
        std::cout << "M5e [3 x 4]\n";
        std::cout << m5e;
    }
    REQUIRE(fabs(m1e[1][1] - 1.0) < 1e-9);
    REQUIRE(fabs(m1e[1][0]) < 1e-9);
    REQUIRE(fabs(m2e[0][1] - 2.0) < 1e-9);
    REQUIRE(fabs(m2e[1][1]) < 1e-9);
    REQUIRE(fabs(m2e[1][2] - 1.0) < 1e-9);
    REQUIRE(fabs(m2e[1][0]) < 1e-9);
    REQUIRE(fabs(m3e[0][0] - 1.0) < 1e-9);
    REQUIRE(fabs(m3e[1][0]) < 1e-9);
    REQUIRE(fabs(m3e[1][1] - 1.0) < 1e-9);
    REQUIRE(fabs(m3e[2][1]) < 1e-9);
    REQUIRE(fabs(m4e[1][1] - 1.0) < 1e-9);
    REQUIRE(fabs(m4e[1][0]) < 1e-9);
    REQUIRE(fabs(m5e[0][3] - 1.0) < 1e-9);
    REQUIRE(fabs(m5e[1][3]) < 1e-9);
    REQUIRE(fabs(m5e[1][3]) < 1e-9);
}

TEST_CASE("[matrix] permutations matrix")
{
    la::Matrix<float> m1{
        std::vector<float>{1, 2, 3, 1, 1, 1, 2, 1, 1, 2, 3, 1},
        3, 4
    };
    la::Matrix<float> m1gj{3, 3};
    la::MatPermutations(m1gj, m1);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "permutations matrix\n";
        std::cout << "M1   [3 x 4]\n";
        std::cout << m1;
        std::cout << "M1GJ [3 x 3]\n";
        std::cout << m1gj;
    }
    REQUIRE(fabs(m1gj[0][1] - 2.0) < 1e-9);
    REQUIRE(fabs(m1gj[2][2] - 1.0) < 1e-9);
}

TEST_CASE("[matrix] column and row basis")
{
    la::Matrix<double> m1{
        std::vector<double>{1, 2, 3, 1, 1, 1, 2, 1, 1, 2, 3, 1},
        3, 4
    };
    la::Matrix<double> m1b{3, 1}, m1br{4, 1};
    la::MatBasis(m1b, m1);
    la::MatBasisRow(m1br, m1);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "column and row basis\n";
        std::cout << "M1   [3 x 4]\n";
        std::cout << m1;
        std::cout << "M1B  [3 x 2]\n";
        std::cout << m1b;
        std::cout << "M1BR [3 x 2]\n";
        std::cout << m1br;
    }
    REQUIRE(fabs(m1b[0][0] - 1.0) < 1e-9);
    REQUIRE(fabs(m1b[1][0] - 1.0) < 1e-9);
    REQUIRE(fabs(m1b[2][0] - 1.0) < 1e-9);
    REQUIRE(fabs(m1b[0][1] - 2.0) < 1e-9);
    REQUIRE(fabs(m1b[1][1] - 1.0) < 1e-9);
    REQUIRE(fabs(m1b[2][1] - 2.0) < 1e-9);
    REQUIRE(fabs(m1br[0][0] - 1.0) < 1e-9);
    REQUIRE(fabs(m1br[1][0]) < 1e-9);
    REQUIRE(fabs(m1br[2][0] - 1.0) < 1e-9);
    REQUIRE(fabs(m1br[3][0] - 1.0) < 1e-9);
    REQUIRE(fabs(m1br[0][1]) < 1e-9);
    REQUIRE(fabs(m1br[1][1] - 1.0) < 1e-9);
    REQUIRE(fabs(m1br[2][1] - 1.0) < 1e-9);
    REQUIRE(fabs(m1br[3][1]) < 1e-9);
}

TEST_CASE("[matrix] nullspace")
{
    la::Matrix<float> m1{
        std::vector<float>{1, 2, 3, 1, 1, 1, 2, 1, 1, 2, 3, 1},
        3, 4
    };
    la::Matrix<float> m2{
        std::vector<float>{0, 4, -4, 8, 2, 4, 0, 2, 3, 0, 6, 9},
        3, 4
    };
    la::Matrix<float> m3{
        std::vector<float>{2, 4, 2, 2, 1, 3, 2, 0, 3, 1, -2, 8},
        3, 4
    };
    la::Matrix<float> m1n{4, 1}, m2n{4, 1}, m3n{4, 1};
    la::MatNullspace(m1n, m1);
    la::MatNullspace(m2n, m2);
    la::MatNullspace(m3n, m3);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "nullspace\n";
        std::cout << "M1  [3 x 4]\n";
        std::cout << m1;
        std::cout << "M1N [4 x 2]\n";
        std::cout << m1n;
        std::cout << "M2  [3 x 4]\n";
        std::cout << m2;
        std::cout << "M2N [4 x 1]\n";
        std::cout << m2n;
        std::cout << "M3  [3 x 4]\n";
        std::cout << m3;
        std::cout << "M3N [4 x 2]\n";
        std::cout << m3n;
    }
    REQUIRE(fabs(m1n[0][0] - 1.0) < 1e-6);
    REQUIRE(fabs(m1n[0][1] - 1.0) < 1e-6);
    REQUIRE(fabs(m1n[2][1]) < 1e-6);
    REQUIRE(fabs(m1n[3][1] + 1.0) < 1e-6);
    REQUIRE(fabs(m2n[0][0] - 2.0) < 1e-6);
    REQUIRE(fabs(m2n[2][0] + 1.0) < 1e-6);
    REQUIRE(fabs(m3n[2][0] + 1.0) < 1e-6);
    REQUIRE(fabs(m3n[0][1] - 3.0) < 1e-6);
}

TEST_CASE("[matrix] left nullspace")
{
    la::Matrix<float> m1{
        std::vector<float>{1, 2, 3, 1, 1, 1, 2, 1, 1, 2, 3, 1},
        3, 4
    };

    la::Matrix<float> m1ln{3, 1};
    la::MatLeftNullspace(m1ln, m1);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "left nullspace\n";
        std::cout << "M1   [3 x 4]\n";
        std::cout << m1;
        std::cout << "M1LN [3 x 1]\n";
        std::cout << m1ln;
    }
    REQUIRE(fabs(m1ln[0][0] + 1.0) < 1e-6);
    REQUIRE(fabs(m1ln[1][0]) < 1e-6);
    REQUIRE(fabs(m1ln[2][0] - 1.0) < 1e-6);
}

TEST_CASE("[matrix] pivot column number")
{
    la::Matrix<float> m1{
        std::vector<float>{1, 2, 3, 1, 1, 2, 3, 1, 1, 1, 2, 1, 1, 2, 3, 1, 1, 2, 3, 2},
        5, 4
    };
    std::vector<size_t> m1pc = la::MatPivotColsNb(m1);
    std::vector<size_t> m1fc = la::MatFreeColsNb(m1);
    // compute the same for the transpose
    la::Matrix<float> m1t{m1.GetColsNb(), m1.GetRowsNb()};
    la::MatTranspose(m1t, m1);
    std::vector<size_t> m1tpc = la::MatPivotColsNb(m1t);
    std::vector<size_t> m1tfc = la::MatFreeColsNb(m1t);

    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "pivot column number\n";
        std::cout << "M1   [3 x 4]\n";
        std::cout << m1;
        std::cout << "M1P  [2]\n";
        for (size_t i = 0; i < m1pc.size(); ++i) std::cout << m1pc[i] << " ";
        std::cout << "\n";
        std::cout << "M1F  [2]\n";
        for (size_t i = 0; i < m1fc.size(); ++i) std::cout << m1fc[i] << " ";
        std::cout << "\n";
        std::cout << "M1T  [4 x 3]\n";
        std::cout << m1t;
        std::cout << "M1TP [2]\n";
        for (size_t i = 0; i < m1tpc.size(); ++i) std::cout << m1tpc[i] << " ";
        std::cout << "\n";
        std::cout << "M1TF [2]\n";
        for (size_t i = 0; i < m1tfc.size(); ++i) std::cout << m1tfc[i] << " ";
        std::cout << "\n";
    }
    REQUIRE(m1pc[0] == 0);
    REQUIRE(m1pc[1] == 1);
    REQUIRE(m1fc[0] == 2);
    REQUIRE(m1fc[1] == 3);
    REQUIRE(m1tpc[0] == 0);
    REQUIRE(m1tpc[1] == 2);
    REQUIRE(m1tpc[2] == 4);
    REQUIRE(m1tfc[0] == 1);
    REQUIRE(m1tfc[1] == 3);
}

TEST_CASE("[matrix] pivot row number")
{
    la::Matrix<float> m1{
        std::vector<float>{1, 2, 3, 1, 1, 2, 3, 1, 1, 1, 2, 1, 1, 2, 3, 1, 1, 2, 3, 2},
        5, 4
    };
    std::vector<size_t> m1pr = la::MatPivotRowsNb(m1);
    std::vector<size_t> m1fr = la::MatFreeRowsNb(m1);
    la::Matrix<float> m1t{m1.GetColsNb(), m1.GetRowsNb()};
    la::MatTranspose(m1t, m1);
    std::vector<size_t> m1tpr = la::MatPivotRowsNb(m1t);
    std::vector<size_t> m1tfr = la::MatFreeRowsNb(m1t);

    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "pivot column number\n";
        std::cout << "M1   [3 x 4]\n";
        std::cout << m1;
        std::cout << "M1R  [3]\n";
        for (size_t i = 0; i < m1pr.size(); ++i) std::cout << m1pr[i] << " ";
        std::cout << "\n";
        std::cout << "M1FR [2]\n";
        for (size_t i = 0; i < m1fr.size(); ++i) std::cout << m1fr[i] << " ";
        std::cout << "\n";
        std::cout << "M1T  [4 x 3]\n";
        std::cout << m1t;
        std::cout << "M1TR [3]\n";
        for (size_t i = 0; i < m1tpr.size(); ++i) std::cout << m1tpr[i] << " ";
        std::cout << "\n";
        std::cout << "M1TF [2]\n";
        for (size_t i = 0; i < m1tfr.size(); ++i) std::cout << m1tfr[i] << " ";
        std::cout << "\n";
    }
    REQUIRE(m1pr[0] == 0);
    REQUIRE(m1pr[1] == 2);
    REQUIRE(m1pr[2] == 4);
    REQUIRE(m1fr[0] == 3);
    REQUIRE(m1fr[1] == 1);
    REQUIRE(m1tpr[0] == 0);
    REQUIRE(m1tpr[1] == 1);
    REQUIRE(m1tpr[2] == 3);
    REQUIRE(m1tfr[0] == 2);
}

TEST_CASE("[matrix] read file")
{
    la::Matrix<float> m1;
    la::MatReadFile(m1, "cpp/test/data/matrix.data");
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1\n";
        std::cout << m1;
    }
    REQUIRE(m1.GetRowsNb() == 3);
    REQUIRE(m1.GetColsNb() == 2);
}

TEST_CASE("[matrix] AtA")
{
    la::Matrix<double> m1{
        std::vector<double>{1, 1, 1, 2, 1, 5},
        3, 2
    };
    la::Matrix<double> m1ata{m1.GetColsNb(), m1.GetColsNb()};
    la::MatAtA(m1ata, m1);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1    [3 x 2]\n";
        std::cout << m1;
        std::cout << "m1ata [3 x 3]\n";
        std::cout << m1ata;
    }
    REQUIRE(m1ata.GetRowsNb() == 2);
    REQUIRE(m1ata.GetColsNb() == 2);
    REQUIRE(m1ata[0][0] == 3);
}

TEST_CASE("[matrix] QR factorization")
{
    la::Matrix<double> m1{
        std::vector<double>{0, 1, 0, 0, 2, -0.2, -2, 0, 1, 0, 0, 1, 0, -0.1, 6, 0},
        4, 4
    },
        q{4, 4}, r{4, 4}, m1qr{4, 4};

    la::MatQR(q, r, m1);
    la::MatMult(m1qr, q, r);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [4 x 4]\n";
        std::cout << m1;
        std::cout << "q    [4 x 4]\n";
        std::cout << q;
        std::cout << "r    [4 x 4]\n";
        std::cout << r;
        std::cout << "m1qr [4 x 4]\n";
        std::cout << m1qr;
    }

    REQUIRE(fabs(q[0][0]) < 1e-6);
    REQUIRE(fabs(q[0][1] - 0.99112) < 1e-6);
    REQUIRE(fabs(q[0][2] - 0.0845093) < 1e-6);
    REQUIRE(fabs(q[0][3] + 0.102664) < 1e-6);
    REQUIRE(fabs(q[1][0] - 0.894427) < 1e-6);
    REQUIRE(fabs(q[1][1] + 0.0396448) < 1e-6);
    REQUIRE(fabs(q[1][2] + 0.0695577) < 1e-6);
    REQUIRE(fabs(q[1][3] + 0.439989) < 1e-6);
    REQUIRE(fabs(q[2][0] - 0.447214) < 1e-6);
    REQUIRE(fabs(q[2][1] - 0.0792896) < 1e-6);
    REQUIRE(fabs(q[2][2] - 0.139115) < 1e-6);
    REQUIRE(fabs(q[2][3] - 0.879977) < 1e-6);
    REQUIRE(fabs(q[3][0]) < 1e-6);
    REQUIRE(fabs(q[3][1] + 0.099112) < 1e-6);
    REQUIRE(fabs(q[3][2] - 0.984209) < 1e-6);
    REQUIRE(fabs(q[3][3] + 0.146663) < 1e-6);
    REQUIRE(fabs(r[0][0] - 2.23607) < 1e-5);
    REQUIRE(fabs(r[0][1] + 0.178885) < 1e-5);
    REQUIRE(fabs(r[0][2] + 1.78885) < 1e-5);
    REQUIRE(fabs(r[0][3] - 0.447214) < 1e-5);
    REQUIRE(fabs(r[1][0]) < 1e-5);
    REQUIRE(fabs(r[1][1] - 1.00896) < 1e-5);
    REQUIRE(fabs(r[1][2] + 0.515382) < 1e-5);
    REQUIRE(fabs(r[1][3] - 0.0792896) < 1e-5);
    REQUIRE(fabs(r[2][0]) < 1e-6);
    REQUIRE(fabs(r[2][1]) < 1e-6);
    REQUIRE(fabs(r[2][2] - 6.04437) < 1e-5);
    REQUIRE(fabs(r[2][3] - 0.139115) < 1e-5);
    REQUIRE(fabs(r[3][0]) < 1e-6);
    REQUIRE(fabs(r[3][1]) < 1e-6);
    REQUIRE(fabs(r[3][2]) < 1e-6);
    REQUIRE(fabs(r[3][3] - 0.879977) < 1e-5);

    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++) REQUIRE(fabs(m1qr[i][j] - m1[i][j]) < 1e-5);

    la::Matrix<double> m2{
        std::vector<double>{0, 1, 0, 0, 0, -0.2, -2, 0, 0, 0, 0, 1, 0, -0.1, 6, 0},
        4, 4
    };

    la::MatQR(q, r, m2);
    la::MatMult(m1qr, q, r);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m2   [4 x 4]\n";
        std::cout << m2;
        std::cout << "q    [4 x 4]\n";
        std::cout << q;
        std::cout << "r    [4 x 4]\n";
        std::cout << r;
        std::cout << "m1qr [4 x 4]\n";
        std::cout << m1qr;
    }

    REQUIRE(fabs(q[0][0]) < 1e-6);
    REQUIRE(fabs(q[0][1] - 0.9759) < 1e-5);
    REQUIRE(fabs(q[0][2] - 0.0301313) < 1e-5);
    REQUIRE(fabs(q[0][3]) < 1e-6);
    REQUIRE(fabs(q[1][0]) < 1e-6);
    REQUIRE(fabs(q[1][1] + 0.19518) < 1e-5);
    REQUIRE(fabs(q[1][2] + 0.322405) < 1e-5);
    REQUIRE(fabs(q[1][3]) < 1e-6);
    REQUIRE(fabs(q[2][0]) < 1e-6);
    REQUIRE(fabs(q[2][1]) < 1e-6);
    REQUIRE(fabs(q[2][2]) < 1e-6);
    REQUIRE(fabs(q[2][3] - 1) < 1e-6);
    REQUIRE(fabs(q[3][0]) < 1e-6);
    REQUIRE(fabs(q[3][1] + 0.09759) < 1e-5);
    REQUIRE(fabs(q[3][2] - 0.946122) < 1e-5);
    REQUIRE(fabs(q[3][3]) < 1e-6);
    REQUIRE(fabs(r[0][0]) < 1e-6);
    REQUIRE(fabs(r[0][1]) < 1e-6);
    REQUIRE(fabs(r[0][2]) < 1e-6);
    REQUIRE(fabs(r[0][3]) < 1e-6);
    REQUIRE(fabs(r[1][0]) < 1e-6);
    REQUIRE(fabs(r[1][1] - 1.0247) < 1e-5);
    REQUIRE(fabs(r[1][2] + 0.19518) < 1e-5);
    REQUIRE(fabs(r[1][3]) < 1e-6);
    REQUIRE(fabs(r[2][0]) < 1e-6);
    REQUIRE(fabs(r[2][1]) < 1e-6);
    REQUIRE(fabs(r[2][2] - 6.32154) < 1e-5);
    REQUIRE(fabs(r[2][3]) < 1e-6);
    REQUIRE(fabs(r[3][0]) < 1e-6);
    REQUIRE(fabs(r[3][1]) < 1e-6);
    REQUIRE(fabs(r[3][2]) < 1e-6);
    REQUIRE(fabs(r[3][3] - 1) < 1e-6);
    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++) REQUIRE(fabs(m1qr[i][j] - m2[i][j]) < 1e-5);
}

TEST_CASE("[matrix] eigenvalues computation")
{
    la::Matrix<double> m1{
        std::vector<double>{0, 1, 0, 0, 2, -0.2, -2, 0, 1, 0, 0, 1, 0, -0.1, 6, 0},
        4, 4
    };
    std::vector<double> eigs;
    la::MatEigen(eigs, m1);

    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [4 x 4]\n";
        std::cout << m1;
        std::cout << "eigs [4]\n";
        for (auto e : eigs) std::cout << e << " ";
        std::cout << std::endl;
    }
    REQUIRE(fabs(eigs[0] + 2.65643) < 1e-5);
    REQUIRE(fabs(eigs[1] - 2.18532) < 1e-5);
    REQUIRE(fabs(eigs[2] - 1.57969) < 1e-5);
    REQUIRE(fabs(eigs[3] + 1.30857) < 1e-5);
    la::Matrix<double> m2{
        std::vector<double>{0, 1, 0, 0, 0, -0.2, -2, 0, 0, 0, 0, 1, 0, -0.1, 6, 0},
        4, 4
    };
    la::MatEigen(eigs, m2);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m2   [4 x 4]\n";
        std::cout << m2;
        std::cout << "eigs [4]\n";
        for (auto e : eigs) std::cout << e << " ";
        std::cout << std::endl;
    }
    REQUIRE(fabs(eigs[0]) < 1e-6);
    REQUIRE(fabs(eigs[1] - 2.46924) < 1e-5);
    REQUIRE(fabs(eigs[2] + 2.4356) < 1e-5);
    REQUIRE(fabs(eigs[3] + 0.233639) < 1e-5);
}

TEST_CASE("[matrix] complex eigenvalues computation")
{
    la::Matrix<double> m1{
        std::vector<double>{0, 1, 1, 0},
        2, 2
    };
    std::vector<std::complex<double>> eigs;
    la::MatEigen(eigs, m1);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [2 x 2]\n";
        std::cout << m1;
        std::cout << "eigs [2]\n";
        for (auto e : eigs) std::cout << e << " ";
        std::cout << std::endl;
    }
    REQUIRE(fabs(eigs[0].real() - 1) < 1e-6);
    REQUIRE(fabs(eigs[0].imag()) < 1e-6);
    REQUIRE(fabs(eigs[1].real() + 1) < 1e-6);
    REQUIRE(fabs(eigs[1].imag()) < 1e-6);
    la::Matrix<double> m2{
        std::vector<double>{0, 1, -1, 0},
        2, 2
    };
    la::MatEigen(eigs, m2);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m2   [2 x 2]\n";
        std::cout << m1;
        std::cout << "eigs [2]\n";
        for (auto e : eigs) std::cout << e << " ";
        std::cout << std::endl;
    }
    REQUIRE(fabs(eigs[0].real()) < 1e-6);
    REQUIRE(fabs(eigs[0].imag() - 1) < 1e-6);
    REQUIRE(fabs(eigs[1].real()) < 1e-6);
    REQUIRE(fabs(eigs[1].imag() + 1) < 1e-6);
    la::Matrix<double> m3{
        std::vector<double>{2, 3, 1, 0.5, 4, 4, 5, 7, 0.1, 1, 5, 3, 6, 19.2, 9, 1, 4, 1, 4, 7, 3, 1, 6, 2, 6},
        5, 5
    };
    la::MatEigen(eigs, m3);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m3   [5 x 5]\n";
        std::cout << m2;
        std::cout << "eigs [5]\n";
        for (auto e : eigs) std::cout << e << " ";
        std::cout << std::endl;
    }
    REQUIRE(fabs(eigs[0].real() - 21.35894) < 1e-5);
    REQUIRE(fabs(eigs[0].imag()) < 1e-5);
    REQUIRE(fabs(eigs[1].real() + 1.98096) < 1e-5);
    REQUIRE(fabs(eigs[1].imag() - 6.6826) < 1e-5);
    REQUIRE(fabs(eigs[2].real() + 1.98096) < 1e-5);
    REQUIRE(fabs(eigs[2].imag() + 6.6826) < 1e-5);
    REQUIRE(fabs(eigs[3].real() - 4.34496) < 1e-5);
    REQUIRE(fabs(eigs[3].imag()) < 1e-5);
    REQUIRE(fabs(eigs[4].real() - 1.25802) < 1e-5);
    REQUIRE(fabs(eigs[4].imag()) < 1e-5);
}

TEST_CASE("[matrix] float orthogonal vectors")
{
    la::Matrix<float> m1{
        std::vector<float>{1, 2, 3, 4},
        4, 1
    };
    la::Matrix<float> m2{
        std::vector<float>{-2, -3, 0, 2},
        4, 1
    };
    bool res = la::MatIsOrthogonal(m1, m2, 1e-5f);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [4 x 1]\n";
        std::cout << la::MatTranspose(m1);
        std::cout << "m2   [4 x 1]\n";
        std::cout << la::MatTranspose(m2);
        std::cout << "m1 * m2 = ";
        std::cout << la::MatMult(MatTranspose(m1), m2);
        std::cout << "m1 and m2 are orthogonal = ";
        res ? std::cout << "true\n" : std::cout << "false\n";
    }
    REQUIRE(res == true);
}

TEST_CASE("[matrix] double orthogonal vectors")
{
    la::Matrix<double> m1{
        std::vector<double>{1, 2, 3, 4},
        4, 1
    };
    la::Matrix<double> m2{
        std::vector<double>{-2, -3, 0, 2},
        4, 1
    };
    bool res = la::MatIsOrthogonal(m1, m2);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [4 x 1]\n";
        std::cout << la::MatTranspose(m1);
        std::cout << "m2   [4 x 1]\n";
        std::cout << la::MatTranspose(m2);
        std::cout << "m1 * m2 = ";
        std::cout << la::MatMult(MatTranspose(m1), m2);
        std::cout << "m1 and m2 are orthogonal = ";
        res ? std::cout << "true\n" : std::cout << "false\n";
    }
    REQUIRE(res == true);
}

TEST_CASE("[matrix] complex float orthogonal vectors")
{
    la::Matrix<std::complex<float>> m1{
        std::vector<std::complex<float>>{{1, 0}, {0, 1}, {-1, 0}, {1, 0}},
        4, 1
    };
    la::Matrix<std::complex<float>> m2{
        std::vector<std::complex<float>>{{2, 0}, {1, 0}, {1, 0}, {-1, 0}},
        4, 1
    };
    bool res = la::MatIsOrthogonal(m1, m2);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [4 x 1]\n";
        std::cout << la::MatTranspose(m1);
        std::cout << "m2   [4 x 1]\n";
        std::cout << la::MatTranspose(m2);
        std::cout << "m1 * m2 = ";
        std::cout << la::MatMult(MatTranspose(la::MatConjugate(m1)), m2);
        std::cout << "m1 and m2 are orthogonal = ";
        res ? std::cout << "true\n" : std::cout << "false\n";
    }
    REQUIRE(res == true);
}

TEST_CASE("[matrix] complex double orthogonal vectors")
{
    la::Matrix<std::complex<double>> m1{
        std::vector<std::complex<double>>{{1, 0}, {0, 1}, {-1, 0}, {1, 0}},
        4, 1
    };
    la::Matrix<std::complex<double>> m2{
        std::vector<std::complex<double>>{{2, 0}, {1, 0}, {1, 0}, {-1, 0}},
        4, 1
    };
    bool res = la::MatIsOrthogonal(m1, m2);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [4 x 1]\n";
        std::cout << la::MatTranspose(m1);
        std::cout << "m2   [4 x 1]\n";
        std::cout << la::MatTranspose(m2);
        std::cout << "m1 * m2 = ";
        std::cout << la::MatMult(MatTranspose(la::MatConjugate(m1)), m2);
        std::cout << "m1 and m2 are orthogonal = ";
        res ? std::cout << "true\n" : std::cout << "false\n";
    }
    REQUIRE(res == true);
}

TEST_CASE("[matrix] float orthonormal matrix")
{
    la::Matrix<float> m1{
        std::vector<float>{2, -2, 1, 1, 2, 2, 2, 1, -2},
        3, 3
    };
    m1 /= 3;
    bool res = la::MatIsOrthogonal(m1, 1e-5f);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [3 x 3]\n";
        std::cout << m1;
        std::cout << "m1 is orthonormal = ";
        res ? std::cout << "true\n" : std::cout << "false\n";
    }
    REQUIRE(res == true);
}

TEST_CASE("[matrix] double orthonormal matrix")
{
    la::Matrix<double> m1{
        std::vector<double>{2, -2, 1, 1, 2, 2, 2, 1, -2},
        3, 3
    };
    m1 /= 3;
    bool res = la::MatIsOrthogonal(m1);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [3 x 3]\n";
        std::cout << m1;
        std::cout << "m1 is orthonormal = ";
        res ? std::cout << "true\n" : std::cout << "false\n";
    }
    REQUIRE(res == true);
}

TEST_CASE("[matrix] complex double orthonormal matrix")
{
    // clang-format off
    la::Matrix<std::complex<float>> m1{
        std::vector<std::complex<float>>{
        {-0.109109f,0.f},{-0.224624f,-0.802702f},{-0.185283f,0.205534f},{-0.287308f,0.366318f},{-0.327327f,-0.f},{-0.118919f,-0.465765f},{0.259474f,-0.393365f},{0.455997f,-0.481437f},{-0.545545f,-0.f},{-0.0132132f,0.148648f},{0.632344f,0.0409754f},{-0.526344f,0.039164f},{-0.763763f,-0.f},{0.0924924f,0.208108f},{-0.536409f,0.109955f},{0.221577f,0.126025f}}, 4, 4};
    // clang-format on
    std::complex<float> tol = {1e-5f, 0};
    bool res                = la::MatIsOrthogonal(m1, tol);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [4 x 4]\n";
        std::cout << m1;
        std::cout << "m1 is orthonormal = ";
        res ? std::cout << "true\n" : std::cout << "false\n";
    }
    REQUIRE(res == true);
}

TEST_CASE("[matrix] complex double orthonormal matrix")
{
    // clang-format off
    la::Matrix<std::complex<double>> m1{
        std::vector<std::complex<double>>{
        {-0.109109,0},{-0.224624,-0.802702},{-0.185283,0.205534},{-0.287308,0.366318},{-0.327327,-0},{-0.118919,-0.465765},{0.259474,-0.393365},{0.455997,-0.481437},{-0.545545,-0},{-0.0132132,0.148648},{0.632344,0.0409754},{-0.526344,0.039164},{-0.763763,-0},{0.0924924,0.208108},{-0.536409,0.109955},{0.221577,0.126025}}, 4, 4};
    // clang-format on
    std::complex<double> tol = {1e-5f, 0};
    bool res                 = la::MatIsOrthogonal(m1, tol);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [4 x 4]\n";
        std::cout << m1;
        std::cout << "m1 is orthonormal = ";
        res ? std::cout << "true\n" : std::cout << "false\n";
    }
    REQUIRE(res == true);
}

TEST_CASE("[matrix] real / imaginary part of a real matrix")
{
    la::Matrix<double> m1{
        std::vector<double>{4.5, 2.0, 0.5, 3.2, 0.0, 7.4, 5.6, 3.2, 0.0, 1.1, 1.3, 7.1},
        3, 4
    };
    la::Matrix<double> r1{3, 4}, i1{3, 4};
    la::MatReal(r1, m1);
    la::MatImag(i1, m1);

    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [3 x 4]\n";
        std::cout << m1;
        std::cout << "real(m1)   [3 x 4]\n";
        std::cout << r1;
        std::cout << "imag(m1)   [3 x 4]\n";
        std::cout << i1;
    }

    REQUIRE(fabs(r1[0][0] - 4.5) < 1e-6);
    REQUIRE(fabs(r1[0][1] - 2.0) < 1e-6);
    REQUIRE(fabs(r1[0][2] - 0.5) < 1e-6);
    REQUIRE(fabs(r1[0][3] - 3.2) < 1e-6);
    REQUIRE(fabs(r1[1][0] - 0.0) < 1e-6);
    REQUIRE(fabs(r1[1][1] - 7.4) < 1e-6);
    REQUIRE(fabs(r1[1][2] - 5.6) < 1e-6);
    REQUIRE(fabs(r1[1][3] - 3.2) < 1e-6);
    REQUIRE(fabs(r1[2][0] - 0.0) < 1e-6);
    REQUIRE(fabs(r1[2][1] - 1.1) < 1e-6);
    REQUIRE(fabs(r1[2][2] - 1.3) < 1e-6);
    REQUIRE(fabs(r1[2][3] - 7.1) < 1e-6);
    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 4; j++) REQUIRE(fabs(i1[i][j]) < 1e-6);
}

TEST_CASE("[matrix] real / imaginary part of a std::complex<double> matrix")
{
    // clang-format off
    la::Matrix<std::complex<double>> m1{
        std::vector<std::complex<double>>{{1.2, 4.2}, {2.3, 5.6}, {3.4, 6.7}, {4.5, 7.8}, {5.6, 8.9}, {6.7, 9.0}},
        2, 3
    };
    // clang-format on
    la::Matrix<double> r1{2, 3}, i1{2, 3};
    la::MatReal(r1, m1);
    la::MatImag(i1, m1);
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [3 x 4]\n";
        std::cout << m1;
        std::cout << "real(m1)   [3 x 4]\n";
        std::cout << r1;
        std::cout << "imag(m1)   [3 x 4]\n";
        std::cout << i1;
    }
    REQUIRE(fabs(r1[0][0] - 1.2) < 1e-6);
    REQUIRE(fabs(r1[0][1] - 2.3) < 1e-6);
    REQUIRE(fabs(r1[0][2] - 3.4) < 1e-6);
    REQUIRE(fabs(r1[1][0] - 4.5) < 1e-6);
    REQUIRE(fabs(r1[1][1] - 5.6) < 1e-6);
    REQUIRE(fabs(r1[1][2] - 6.7) < 1e-6);
    REQUIRE(fabs(i1[0][0] - 4.2) < 1e-6);
    REQUIRE(fabs(i1[0][1] - 5.6) < 1e-6);
    REQUIRE(fabs(i1[0][2] - 6.7) < 1e-6);
    REQUIRE(fabs(i1[1][0] - 7.8) < 1e-6);
    REQUIRE(fabs(i1[1][1] - 8.9) < 1e-6);
    REQUIRE(fabs(i1[1][2] - 9.0) < 1e-6);
}

TEST_CASE("[matrix] real / imaginary part of a std::complex<double> matrix")
{
    // clang-format off
    la::Matrix<std::complex<float>> m1{
        std::vector<std::complex<float>>{{1.2f, 4.5f}, {2.3f, 5.6f}, {3.4f, 6.7f}, {4.5f, 7.8f}, {5.6f, 8.9f}, {6.7f, 9.0f}},
        2, 3
    };
    // clang-format on
    la::Matrix<std::complex<float>> f1{2, 3};
    CHECK_THROWS_AS(la::MatReal(f1, m1), std::runtime_error);
    CHECK_THROWS_WITH(la::MatReal(f1, m1), doctest::Contains("MatReal: invalid type For T"));
    CHECK_THROWS_AS(la::MatImag(f1, m1), std::runtime_error);
    CHECK_THROWS_WITH(la::MatImag(f1, m1), doctest::Contains("MatImag: invalid type For T"));
    if (GLOBAL_VERBOSE_FLAG)
    {
        std::cout << "m1   [3 x 4]\n";
        std::cout << m1;
        std::cout << "real(m1) is throwing an error\n";
        std::cout << "imag(m1) is throwing an error\n";
    }
}

TEST_SUITE_END();

#endif
