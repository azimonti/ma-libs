/******************/
/*   example.cpp  */
/*  Version 1.0   */
/*   2025/05/07   */
/******************/

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include "example.h"

// Implementation for Example class
Example::Example(const std::string& name) : name_(name) {}

void Example::greet()
{
    std::cout << "Hello, " << name_ << "!" << std::endl;
}

std::string Example::getName() const
{
    return name_;
}

void Example::setName(const std::string& name)
{
    name_ = name;
}

// Implementation for VectorWriter template class
template <typename T> VectorWriter<T>::VectorWriter()
{
    // Fixed seed for reproducibility
    std::mt19937 gen(42);
    std::uniform_real_distribution<T> dist(0.0, 1.0);

    data2_.resize(4);
    for (T& val : data2_) { val = dist(gen); }
}

template <typename T> std::vector<T> VectorWriter<T>::getVector() const
{
    return data2_;
}

template <typename T> void VectorWriter<T>::printVector() const
{
    std::cout << "Vector elements: ";
    for (size_t i = 0; i < data2_.size(); ++i) { std::cout << data2_[i] << (i == data2_.size() - 1 ? "" : ", "); }
    std::cout << std::endl;
}

template <typename T> void VectorWriter<T>::writeVectorToMemory(T* out_array, size_t size) const
{
    if (size < data2_.size()) { throw std::runtime_error("Output array size is smaller than data2_ vector size."); }
    std::copy(data2_.begin(), data2_.end(), out_array);
}

// --- Methods for data_ (array) ---
template <typename T> std::vector<T> VectorWriter<T>::getArray() const
{
    return std::vector<T>(std::begin(data_), std::end(data_));
}

template <typename T> void VectorWriter<T>::printArray() const
{
    std::cout << "Array elements: ";
    for (size_t i = 0; i < 3; ++i) { std::cout << data_[i] << (i == 2 ? "" : ", "); }
    std::cout << std::endl;
}

template <typename T> void VectorWriter<T>::writeArrayToMemory(T* out_array, size_t size) const
{
    if (size < 3) { throw std::runtime_error("Output array size is smaller than internal array size (3)."); }
    std::copy(std::begin(data_), std::end(data_), out_array);
}

// Explicit template instantiations
template class VectorWriter<float>;
template class VectorWriter<double>;
