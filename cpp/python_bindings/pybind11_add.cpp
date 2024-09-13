/************************/
/*  pybind11_add.cpp    */
/*    Version 1.0       */
/*     2021/03/30       */
/************************/

#include <iostream>
#include "pybinding_test.h"

PYBIND11_MODULE(pybind11_add, m)
{
    m.doc()                     = "pybind11 example plugin";

    m.attr("test_integer")      = 1;
    pybind11::object one_string = pybind11::cast("This is a string");
    m.attr("test_string")       = one_string;

    m.def("add", &add, "A function which adds two numbers", pybind11::arg("i") = 1, pybind11::arg("j") = 1);
    pybind11::class_<PyTest>(m, "PyTest")
        .def(pybind11::init<>())
        .def("print_test", &PyTest::test_cout)
        .def("n0", &PyTest::n0)
        .def("n0_2", &PyTest::n0_2)
        .def("n2", &PyTest::n2)
        .def("array_int", &PyTest::array_int)
        .def("vector_double", &PyTest::vector_double);
}
