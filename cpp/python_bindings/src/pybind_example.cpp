/************************/
/*  pybind_example.cpp  */
/*     Version 1.1      */
/*      2025/05/09      */
/************************/

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "example.h"
#include "example_py.h"

namespace py = pybind11;

// Helper function to bind PyVectorWriter<T>
template <typename T, typename PyClass> void bind_py_vector_writer_derived(PyClass& cls)
{ // Renamed to avoid conflict if a similar helper existed
    cls.def(py::init<>())
        .def("array_t", &PyVectorWriter<T>::array_t, py::return_value_policy::reference_internal,
             "Returns a memory view of the internal C-style array (data_).")
        .def("vector_t", &PyVectorWriter<T>::vector_t, py::return_value_policy::reference_internal,
             "Returns a memory view of the internal std::vector (data2_).")
        // Inherited methods are automatically available if the base class (VectorWriter<T>)
        // is also exposed or if they are public and the binding specifies inheritance.
        // If VectorWriter<T> itself is not bound, or to be explicit:
        .def("getVector", &PyVectorWriter<T>::getVector)
        .def("printVector", &PyVectorWriter<T>::printVector)
        .def("getArray", &PyVectorWriter<T>::getArray)
        .def("printArray", &PyVectorWriter<T>::printArray)
        .def("writeArrayToMemory",
             [](const PyVectorWriter<T>& self, py::array_t<T, py::array::c_style | py::array::forcecast> output_array) {
        py::buffer_info buf_info = output_array.request();
        if (buf_info.ndim != 1) { throw std::runtime_error("Output array must be 1-dimensional."); }
        T* ptr      = static_cast<T*>(buf_info.ptr);
        size_t size = static_cast<size_t>(buf_info.shape[0]);
        self.writeArrayToMemory(ptr, size);
    }, py::arg("output_array").noconvert(), "Copies the internal array content to a pre-allocated NumPy array.")
        .def("writeVectorToMemory",
             [](const PyVectorWriter<T>& self, py::array_t<T, py::array::c_style | py::array::forcecast> output_array) {
        py::buffer_info buf_info = output_array.request();
        if (buf_info.ndim != 1) { throw std::runtime_error("Output array must be 1-dimensional."); }
        T* ptr      = static_cast<T*>(buf_info.ptr);
        size_t size = static_cast<size_t>(buf_info.shape[0]);
        self.writeVectorToMemory(ptr, size);
    }, py::arg("output_array").noconvert(), "Copies the internal vector content to a pre-allocated NumPy array.");
}

PYBIND11_MODULE(example_module, m)
{
    m.doc() = "pybind11 example plugin with unified VectorWriter offering memory views";

    py::class_<Example>(m, "Example")
        .def(py::init<const std::string&>())
        .def("greet", &Example::greet)
        .def("getName", &Example::getName)
        .def("setName", &Example::setName)
        .def_property("name", &Example::getName, &Example::setName);

    // First, register the C++ base classes VectorWriter<T> with pybind11.
    // This makes the C++ type known to pybind11's type system.
    // We give them "internal" Python names as they are not the primary interface.
    py::class_<VectorWriter<float>>(m, "_VectorWriterFloatInternalBase")
        .def(py::init<>()); // Minimal binding, just needs to be known.

    py::class_<VectorWriter<double>>(m, "_VectorWriterDoubleInternalBase").def(py::init<>()); // Minimal binding.

    // Now, bind PyVectorWriter<T>, specifying VectorWriter<T> as its C++ base.
    // These will be the classes exposed in Python with the desired names.
    // PyVectorWriter<float> will be "VectorWriterFloat" in Python.
    auto vwf = py::class_<PyVectorWriter<float>, VectorWriter<float>>(m, "VectorWriterFloat");
    bind_py_vector_writer_derived<float>(vwf); // Helper adds methods to PyVectorWriter<float> binding

    // PyVectorWriter<double> will be "VectorWriterDouble" in Python.
    auto vwfd = py::class_<PyVectorWriter<double>, VectorWriter<double>>(m, "VectorWriterDouble");
    bind_py_vector_writer_derived<double>(vwfd); // Helper adds methods to PyVectorWriter<double> binding
}
