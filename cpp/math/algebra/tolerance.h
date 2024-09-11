#ifndef _TOLERANCE_H_74C35FCA095D4BD787A5B85A70F2850F_
#define _TOLERANCE_H_74C35FCA095D4BD787A5B85A70F2850F_

/************************/
/*   tolerance.h        */
/*    Version 1.0       */
/*     2023/06/17       */
/************************/

#include <complex>
#include <iostream>
#include <type_traits>
#include <typeinfo>

namespace la
{

    template <typename T> T DefaultTol()
    {
        if constexpr (std::is_same_v<T, float> || std::is_same_v<T, std::complex<float>>) return static_cast<T>(1e-6f);
        else return static_cast<T>(1e-9);
    }
} // namespace la

#endif
