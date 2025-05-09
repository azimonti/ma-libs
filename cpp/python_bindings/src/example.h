#ifndef _EXAMPLE_H_24351BDFE1814A9F88DCA2FBF9A75202_
#define _EXAMPLE_H_24351BDFE1814A9F88DCA2FBF9A75202_

/******************/
/*   example.h    */
/*  Version 1.0   */
/*   2025/05/07   */
/******************/

#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// Standard class for basic input/output
class Example
{
  public:
    Example(const std::string& name);
    void greet();
    std::string getName() const;
    void setName(const std::string& name);

  private:
    std::string name_;
};

// Template class to write an array and a vector of 4 elements
template <typename T> class VectorWriter
{
  public:
    VectorWriter();

    // Methods for data_ (array)
    std::vector<T> getArray() const; // Return as std::vector for convenience
    void printArray() const;
    void writeArrayToMemory(T* out_array, size_t size) const;

    // Methods for data2_ (vector)
    std::vector<T> getVector() const;
    void printVector() const;
    void writeVectorToMemory(T* out_array, size_t size) const;

  protected: // Changed from private to allow derived class access
    T data_[3]{static_cast<T>(22), static_cast<T>(56), static_cast<T>(87)};
    std::vector<T> data2_;
};

#endif
