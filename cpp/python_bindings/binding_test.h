#ifndef _BINDING_TEST_H_9B674BA51D7C4A1B996665618F54FC51_
#define _BINDING_TEST_H_9B674BA51D7C4A1B996665618F54FC51_

/**********************/
/*  binding_test.h    */
/*    Version 1.0     */
/*     2021/04/04     */
/**********************/

#include <iostream>
#include <vector>
#include "shared_libraries_export.hpp"

extern "C" DLL_PUBLIC int add(int i = 1, int j = 1);

class DLL_PUBLIC CppTest
{
  public:
    CppTest();
    ~CppTest();
    void test_cout();
    void int_array_cout();
    void vector_double_cout();
    int n0() const;
    const int& n0_2() const;
    uint32_t n2() const;
    // the follow throw a runtime error
    // const uint32_t& n3() const;

  protected:
    int n[3]{22, 56, 87};
    std::vector<double> random_doubles;
    static constexpr uint32_t _n2 = 4;
};

#endif
