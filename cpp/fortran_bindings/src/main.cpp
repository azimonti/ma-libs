/************************/
/*     main.cpp         */
/*    Version 1.0       */
/*     2022/06/05       */
/************************/

#define ST_C(x) static_cast<std::size_t>(x)
#ifdef _MSC_VER
#define add_ ADD
#define exec_ EXEC
#endif

extern "C" double add_(int*, double[], int*, double[]);
extern "C" void exec_(void (*)(double*, double*));

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

void op(double* x, double* f)
{
    *f = std::pow(*x, 2);
}

int main()
{

    int x, y;
    double z;
    std::vector<double> ar1 = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> ar2 = {5.0, 6.0, 7.0, 8.0};

    x                       = 3;
    y                       = 3;

    /* Note: Fortran indexes arrays 1..n */
    /* C indexes arrays 0..(n-1) */
    z                       = add_(&x, ar1.data(), &y, ar2.data());
    std::cout << "The sum of " << std::fixed << std::setprecision(0) << ar1[ST_C(x - 1)] << " and " << ar2[ST_C(y - 1)]
              << " is " << z << std::endl;
    std::cout << "Expected is " << (ar1[ST_C(x - 1)] + ar2[ST_C(y - 1)]) << std::endl;
    exec_(&op);
    std::cout << "Expected is 3^2 = 9 " << std::endl;
    return 0;
}

#undef ST_C
