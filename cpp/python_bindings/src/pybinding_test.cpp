/************************/
/*  pybinding_test.cpp  */
/*    Version 1.0       */
/*     2021/04/04       */
/************************/

#include "pybinding_test.h"

PyTest::PyTest() {};

PyTest::~PyTest() {};

pybind11::memoryview PyTest::array_int()
{
    return pybind11::memoryview::from_memory(n, sizeof(uint32_t) * 4, false);
};

pybind11::memoryview PyTest::vector_double()
{
    // Create a memoryview from the std::vector<double>
    return pybind11::memoryview::from_buffer(random_doubles.data(), {4}, // Shape (size of the vector)
                                             {sizeof(double)});          // Stride
}
