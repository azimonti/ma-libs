/***************************/
/*  external_function.cpp  */
/*    Version 1.0          */
/*     2022/06/06          */
/***************************/

#include <iostream>
#include <memory>
#include "external_function.h"

void cfun_(int* idim)
{
    std::cout << "***Inside C++ function before creating C++ Object" << std::endl;
    int i   = *idim;
    auto c1 = std::make_unique<class_1<int>>(10, 30);
    c1->store(&i);
    c1->print();
    std::cout << "***Inside C++ function after creating C++ Object" << std::endl;
}

void cfun1_(int* idim1)
{
    std::cout << "***Inside C++ function cfun1 before creating C++ Object" << std::endl;
    int i = *idim1;
    (void)i; // suppress unused variable warning
    auto c2 = std::make_unique<class_2<double>>(40, 50.54);
    c2->print();
    std::cout << "***Inside C++ function after creating C++ temp object" << std::endl;
}
