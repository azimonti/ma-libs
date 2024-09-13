/************************/
/*  binding_test.cpp    */
/*    Version 1.0       */
/*     2021/04/04       */
/************************/
#include <random>
#include "binding_test.h"

int add(int i, int j)
{
    return i + j;
}

CppTest::CppTest()
{
    // Fixed seed for reproducibility
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    random_doubles.resize(4);
    for (double& val : random_doubles) { val = dist(gen); }
}

CppTest::~CppTest() {};

void CppTest::test_cout()
{
    std::cout << "Test String" << '\n';
};

int CppTest::n0() const
{
    return n[0];
};

const int& CppTest::n0_2() const
{
    return n[0];
};

uint32_t CppTest::n2() const
{
    return _n2;
};

// const uint32_t& CppTest::n3() const{
//     return _n2;
// };

void CppTest::int_array_cout()
{
    std::cout << "Integer array n: ";
    for (int i = 0; i < 3; ++i) { std::cout << n[i] << " "; }
    std::cout << std::endl;
}

void CppTest::vector_double_cout()
{
    std::cout << "Double vector: ";
    for (const auto& val : random_doubles) { std::cout << val << " "; }
    std::cout << std::endl;
}
