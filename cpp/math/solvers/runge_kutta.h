#ifndef _RUNGE_KUTTA_H_14EB4EF789D444B9805A5B7F483BE5F7_
#define _RUNGE_KUTTA_H_14EB4EF789D444B9805A5B7F483BE5F7_

/************************/
/*   runge_kutta.h      */
/*    Version 1.0       */
/*     2023/04/11       */
/************************/

#include <array>
#include <cmath>
#include <cassert>

#define S_C(x) static_cast<S>(x)
#define T_C(x) static_cast<T>(x)

namespace ma
{
    template <typename T, typename S> inline S rk4singlestep(S (*fun)(T, S), T dt, T t0, S y0)
    {
        S f1   = fun(t0, y0);
        S f2   = fun(t0 + dt / 2, y0 + S_C(dt / 2) * f1);
        S f3   = fun(t0 + dt / 2, y0 + S_C(dt / 2) * f2);
        S f4   = fun(t0 + dt, y0 + S_C(dt) * f3);
        S yout = y0 + S_C(dt / 6) * (f1 + 2 * f2 + 2 * f3 + f4);
        return yout;
    }

    // create an array version of rk4singlestep
    template <typename T, typename S, typename U, size_t N>
    inline std::array<S, N>&
    rk4singlestep(std::array<S, N>& (*fun)(T, const std::array<S, N>&, std::array<S, N>&, const U&), T dt, T t0,
                  const std::array<S, N>& y0, std::array<S, N>& yout, const U& params)
    {
        std::array<S, N> f1, f1t, f2, f2t, f3, f3t, f4;
        fun(t0, y0, f1, params);
        for (size_t i = 0; i < N; i++) { f1t[i] = y0[i] + S_C(dt / 2) * f1[i]; }
        fun(t0 + dt / 2, f1t, f2, params);
        for (size_t i = 0; i < N; i++) { f2t[i] = y0[i] + S_C(dt / 2) * f2[i]; }
        fun(t0 + dt / 2, f2t, f3, params);
        for (size_t i = 0; i < N; i++) { f3t[i] = y0[i] + S_C(dt) * f3[i]; }
        fun(t0 + dt, f3t, f4, params);
        for (size_t i = 0; i < N; i++) { yout[i] = y0[i] + S_C(dt / 6) * (f1[i] + 2 * f2[i] + 2 * f3[i] + f4[i]); }

        return yout;
    }

    // create a loop version of rk4singlestep called rk4loop
    template <typename T, typename S, typename U, size_t N>
    inline std::vector<std::array<S, N>>&
    rk4loop(std::array<float, N>& (*fun)(T, const std::array<S, N>&, std::array<S, N>&, const U&),std::vector<T> t, std::vector<std::array<S, N>>& yout, const U& params)
    {
        assert(t.size() == yout.size());
        std::array<S, N> f1, f1t, f2, f2t, f3, f3t, f4;
        T dt;
        for(size_t i=1; i<t.size(); i++)
        {
            dt = t[i] - t[i-1];
            fun(t[i-1], yout[i-1], f1, params);
            for (size_t j = 0; j < N; j++) { f1t[j] = yout[i-1][j] + S_C(dt / 2) * f1[j]; }
            fun(t[i-1] + dt / 2, f1t, f2, params);
            for (size_t j = 0; j < N; j++) { f2t[j] = yout[i-1][j] + S_C(dt / 2) * f2[j]; }
            fun(t[i-1] + dt / 2, f2t, f3, params);
            for (size_t j = 0; j < N; j++) { f3t[j] = yout[i-1][j] + S_C(dt) * f3[j]; }
            fun(t[i-1] + dt, f3t, f4, params);
            for (size_t j = 0; j < N; j++) { yout[i][j] = yout[i-1][j] + S_C(dt / 6) * (f1[j] + 2 * f2[j] + 2 * f3[j] + f4[j]);
            }
        }
        return yout;
    }
} // namespace ma

#undef T_C
#undef S_C

#endif
