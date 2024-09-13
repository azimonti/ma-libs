#ifndef _EXTERNAL_FUNCTION_H_AB106E75AFF44BA697EEE2E8C752BB2A_
#define _EXTERNAL_FUNCTION_H_AB106E75AFF44BA697EEE2E8C752BB2A_
/***************************/
/*  external_function.h    */
/*    Version 1.0          */
/*     2022/06/06          */
/***************************/

#ifdef _MSC_VER
#define cfun_ CFUN
#define cfun1_ CFUN1
#endif

#include <iostream>
#include "shared_libraries_export.hpp"

extern "C" DLL_PUBLIC void cfun_(int* idim1);
extern "C" DLL_PUBLIC void cfun1_(int* idim1);

template <class T> class class_1
{
  public:
    class_1(int i, T j) : inter(i), templ_mem(j) { std::cout << "***Inside C++ class_1 constructor" << std::endl; }

    ~class_1() { std::cout << "***Inside C++ class_1 Destructor" << std::endl; }

    void store(T* val) { stor_val = *val; }

    void print(void) { std::cout << inter << "\t" << templ_mem << "\t" << stor_val << std::endl; }

  private:
    int inter;
    T templ_mem;
    T stor_val;
};

template <class T> class class_2
{
  public:
    class_2(int i, T j) : internal(i), temp_var(j) { std::cout << "***Inside C++ class_2 Constructor" << std::endl; }

    ~class_2() { std::cout << "***Inside C++ class_2 destructor" << std::endl; }

    void print(void) { std::cout << internal << "\t" << temp_var << std::endl; }

  private:
    int internal;
    T temp_var;
};
#endif
