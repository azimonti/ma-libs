/*************************/
/*      main.cpp         */
/*     Version 1.0       */
/*      2021/04/04       */
/*************************/

#include "binding_test.h"

int main()
{
    std::cout << "Hello World!" << '\n';
    std::cout << add(3, 4) << '\n';
    CppTest t;
    t.test_cout();
    std::cout << t.n0() << '\n';
    t.int_array_cout();
    t.vector_double_cout();
    return 0;
}
