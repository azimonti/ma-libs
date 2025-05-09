/******************/
/* example_py.cpp */
/*  Version 1.0   */
/*   2025/05/09   */
/******************/

#include <vector>
#include "example_py.h"

// Constructor
template <typename T> PyVectorWriter<T>::PyVectorWriter() : VectorWriter<T>()
{
    // Base class constructor is called automatically
    // Initialization of data_ and data2_ happens in VectorWriter<T>()
}

// Destructor
template <typename T> PyVectorWriter<T>::~PyVectorWriter() {}

// Method to expose data_ (C-style array) as a memory view
template <typename T> pybind11::memoryview PyVectorWriter<T>::array_t()
{
    // VectorWriter<T>::data_ is T data_[3]
    return pybind11::memoryview::from_memory(this->data_,   // Pointer to data
                                             sizeof(T) * 3, // Total size in bytes
                                             true           // Read-only, set to false if writable
    );
    // For a more structured array (e.g., if you want shape and strides):
    // return pybind11::buffer_info(
    //     this->data_,                                /* Pointer to buffer */
    //     sizeof(T),                                 /* Size of one scalar */
    //     pybind11::format_descriptor<T>::format(),  /* Python struct-style format descriptor */
    //     1,                                         /* Number of dimensions */
    //     {3},                                       /* Buffer dimensions */
    //     {sizeof(T)}                                /* Strides (in bytes) for each index */
    // ).cast<pybind11::memoryview>(); // This creates a NumPy-style array view
}

// Method to expose data2_ (std::vector) as a memory view
template <typename T> pybind11::memoryview PyVectorWriter<T>::vector_t()
{
    // VectorWriter<T>::data2_ is std::vector<T>
    // Ensure data2_ is not empty and has data
    if (this->data2_.empty())
    {
        // Explicitly cast nullptr to const void* to resolve ambiguity
        return pybind11::memoryview::from_memory(static_cast<const void*>(nullptr), 0);
    }
    return pybind11::memoryview::from_buffer(this->data2_.data(),                                   // Pointer to data
                                             {static_cast<pybind11::ssize_t>(this->data2_.size())}, // Shape
                                             {static_cast<pybind11::ssize_t>(sizeof(T))}            // Strides
    );
}

// Explicit template instantiations
template class PyVectorWriter<float>;
template class PyVectorWriter<double>;
