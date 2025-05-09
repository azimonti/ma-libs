#ifndef _EXAMPLE_PY_H_24351BDFE1814A9F88DCA2FBF9A75202_
#define _EXAMPLE_PY_H_24351BDFE1814A9F88DCA2FBF9A75202_

/******************/
/*  example_py.h  */
/*  Version 1.0   */
/*   2025/05/09   */
/******************/

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include "example.h"

namespace py = pybind11;

template <typename T> class PyVectorWriter : public VectorWriter<T>
{
  public:
    PyVectorWriter();
    ~PyVectorWriter();

    // Method to expose data_ (array) as a memory view
    // This will access the inherited protected member VectorWriter<T>::data_
    py::memoryview array_t();

    // Method to expose data2_ (vector) as a memory view
    // This will access the inherited protected member VectorWriter<T>::data2_
    py::memoryview vector_t();
};

#endif // _EXAMPLE_PY_H_
