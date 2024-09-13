#ifndef _PYBINDING_TEST_H_553A1609016E4FFE9D58E317669E67CB1_
#define _PYBINDING_TEST_H_553A1609016E4FFE9D58E317669E67CB1_

/**********************/
/*  pybinding_test.h  */
/*    Version 1.0     */
/*     2021/04/04     */
/**********************/

#include <pybind11/pybind11.h>
#include "binding_test.h"

class PyTest : public CppTest
{
  public:
    PyTest();
    ~PyTest();
    pybind11::memoryview array_int();
    pybind11::memoryview vector_double();
};

#endif
