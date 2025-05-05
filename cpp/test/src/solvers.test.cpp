#ifndef __clang_analyzer__
/**************************/
/*   solvers.test.cpp     */
/*    Version 1.0         */
/*     2023/01/29         */
/**************************/

#include <array>
#include <vector>
#include "solvers/runge_kutta.h"
#include "std/cout.h"
#include "doctest.h"
#include "global_static.h"

namespace rk
{
    float testfun1(double t, float y)
    {
        (void)t;
        return -y;
    }

    struct fun_params
    {
        float a;
        float b;
    };

    std::array<float, 2>& testfun2(double t, const std::array<float, 2>& y, std::array<float, 2>& dydt,
                                   const fun_params& params)
    {
        (void)t;
        dydt[0] = -params.a * y[1];
        dydt[1] = params.b * y[0];
        return dydt;
    }

    std::array<float, 2>& testfun3(double t, const std::array<float, 2>& y, std::array<float, 2>& dydt,
                                   const fun_params& params)
    {
        (void)t;
        dydt[0] = -params.a * y[1];
        dydt[1] = params.b * y[0];
        return dydt;
    }

} // namespace rk

TEST_SUITE_BEGIN("[runge_kutta]");

TEST_CASE("[runge_kutta] rk4singlestep")
{
    float (*fun)(double t, float y) = rk::testfun1;
    // define the initial conditions
    double t0                       = 0;
    float y0                        = 1;
    // define the time step
    double dt                       = 0.1;
    // define the number of steps
    size_t nsteps                   = 10;
    // define the output vector
    std::vector<float> yout(nsteps);
    // define the time vector
    std::vector<double> tout(nsteps);
    // define the initial conditions
    yout[0] = y0;
    tout[0] = t0;
    // loop over the steps
    for (size_t i = 1; i < nsteps; i++)
    {
        yout[i] = ma::rk4singlestep(fun, dt, tout[i - 1], yout[i - 1]);
        tout[i] = tout[i - 1] + dt;
    }
    if (GLOBAL_VERBOSE_FLAG)
        for (size_t i = 0; i < nsteps; i++) { std::cout << tout[i] << " " << yout[i] << std::endl; }
    REQUIRE(fabs(yout[0] - 1.f) < 1e-6);
    REQUIRE(fabs(yout[1] - 0.904837f) < 1e-6);
    REQUIRE(fabs(yout[2] - 0.818731f) < 1e-6);
    REQUIRE(fabs(yout[3] - 0.740818f) < 1e-6);
    REQUIRE(fabs(yout[4] - 0.67032f) < 1e-6);
    REQUIRE(fabs(yout[5] - 0.606531f) < 1e-6);
    REQUIRE(fabs(yout[6] - 0.548812f) < 1e-6);
    REQUIRE(fabs(yout[7] - 0.496586f) < 1e-6);
    REQUIRE(fabs(yout[8] - 0.449329f) < 1e-6);
    REQUIRE(fabs(yout[9] - 0.40657f) < 1e-6);
    REQUIRE(fabs(tout[0]) < 1e-6);
    REQUIRE(fabs(tout[9] - 0.9) < 1e-6);
}

TEST_CASE("[runge_kutta] rk4singlestep array")
{
    std::array<float, 2>& (*fun)(double, const std::array<float, 2>&, std::array<float, 2>&, const rk::fun_params&) =
        rk::testfun2;
    // define the parameters
    rk::fun_params params   = {1, 2};
    // define the initial conditions
    double t0               = 0;
    std::array<float, 2> y0 = {1, 2};
    // define the time step
    double dt               = 0.1;
    // define the number of steps
    size_t nsteps           = 10;
    // define the output array
    std::vector<std::array<float, 2>> yout(nsteps);
    // define the time vector
    std::vector<double> tout(nsteps);
    // define the initial conditions
    tout[0]    = t0;
    yout[0][0] = y0[0];
    yout[0][1] = y0[1];
    // loop over the steps
    for (size_t i = 1; i < nsteps; i++)
    {
        ma::rk4singlestep(fun, dt, tout[i - 1], yout[i - 1], yout[i], params);
        tout[i] = tout[i - 1] + dt;
    }
    // print the results
    if (GLOBAL_VERBOSE_FLAG)
        for (size_t i = 0; i < nsteps; i++) std::cout << tout[i] << " " << yout[i][0] << " " << yout[i][1] << std::endl;
    REQUIRE(fabs(yout[0][0] - 1.f) < 1e-6);
    REQUIRE(fabs(yout[0][1] - 2.f) < 1e-6);
    REQUIRE(fabs(yout[1][0] - 0.790683f) < 1e-5);
    REQUIRE(fabs(yout[1][1] - 2.17937f) < 1e-5);
    REQUIRE(fabs(yout[2][0] - 0.565579f) < 1e-5);
    REQUIRE(fabs(yout[2][1] - 2.31522f) < 1e-5);
    REQUIRE(fabs(yout[3][0] - 0.329183f) < 1e-5);
    REQUIRE(fabs(yout[3][1] - 2.40484f) < 1e-5);
    REQUIRE(fabs(yout[4][0] - 0.0862138f) < 1e-5);
    REQUIRE(fabs(yout[4][1] - 2.44645f) < 1e-5);
    REQUIRE(fabs(yout[5][0] - -0.158477f) < 1e-5);
    REQUIRE(fabs(yout[5][1] - 2.43921f) < 1e-5);
    REQUIRE(fabs(yout[6][0] - -0.400003f) < 1e-5);
    REQUIRE(fabs(yout[6][1] - 2.38327f) < 1e-5);
    REQUIRE(fabs(yout[7][0] - -0.633543f) < 1e-5);
    REQUIRE(fabs(yout[7][1] - 2.27975f) < 1e-5);
    REQUIRE(fabs(yout[8][0] - -0.854432f) < 1e-5);
    REQUIRE(fabs(yout[8][1] - 2.1307f) < 1e-5);
    REQUIRE(fabs(yout[9][0] - -1.05826f) < 1e-5);
    REQUIRE(fabs(yout[9][1] - 1.93911f) < 1e-5);
    REQUIRE(fabs(tout[0]) < 1e-6);
    REQUIRE(fabs(tout[9] - 0.9) < 1e-6);
}

TEST_CASE("[runge_kutta] rk4loop")
{
    // define the function
    std::array<float, 2>& (*fun)(double, const std::array<float, 2>&, std::array<float, 2>&, const rk::fun_params&) =
        rk::testfun3;
    // define the parameters
    rk::fun_params params   = {1, 2};
    // define the initial conditions
    double t0               = 0;
    std::array<float, 2> y0 = {1, 2};
    // define the time step
    double dt               = 0.1;
    // define the number of steps
    size_t nsteps           = 10;
    // define the output array
    std::vector<std::array<float, 2>> yout(nsteps);
    // define the time vector
    std::vector<double> tout(nsteps);
    // define the initial conditions
    tout[0]    = t0;
    yout[0][0] = y0[0];
    yout[0][1] = y0[1];
    // loop over the steps
    for (size_t i = 1; i < nsteps; i++) tout[i] = tout[i - 1] + dt;
    ma::rk4loop(fun, tout, yout, params);
    // print the results
    if (GLOBAL_VERBOSE_FLAG)
        for (size_t i = 0; i < nsteps; i++) std::cout << tout[i] << " " << yout[i][0] << " " << yout[i][1] << std::endl;
    REQUIRE(fabs(yout[0][0] - 1.f) < 1e-6);
    REQUIRE(fabs(yout[0][1] - 2.f) < 1e-6);
    REQUIRE(fabs(yout[1][0] - 0.790683f) < 1e-5);
    REQUIRE(fabs(yout[1][1] - 2.17937f) < 1e-5);
    REQUIRE(fabs(yout[2][0] - 0.565579f) < 1e-5);
    REQUIRE(fabs(yout[2][1] - 2.31522f) < 1e-5);
    REQUIRE(fabs(yout[3][0] - 0.329183f) < 1e-5);
    REQUIRE(fabs(yout[3][1] - 2.40484f) < 1e-5);
    REQUIRE(fabs(yout[4][0] - 0.0862138f) < 1e-5);
    REQUIRE(fabs(yout[4][1] - 2.44645f) < 1e-5);
    REQUIRE(fabs(yout[5][0] - -0.158477f) < 1e-5);
    REQUIRE(fabs(yout[5][1] - 2.43921f) < 1e-5);
    REQUIRE(fabs(yout[6][0] - -0.400003f) < 1e-5);
    REQUIRE(fabs(yout[6][1] - 2.38327f) < 1e-5);
    REQUIRE(fabs(yout[7][0] - -0.633543f) < 1e-5);
    REQUIRE(fabs(yout[7][1] - 2.27975f) < 1e-5);
    REQUIRE(fabs(yout[8][0] - -0.854432f) < 1e-5);
    REQUIRE(fabs(yout[8][1] - 2.1307f) < 1e-5);
    REQUIRE(fabs(yout[9][0] - -1.05826f) < 1e-5);
    REQUIRE(fabs(yout[9][1] - 1.93911f) < 1e-5);
    REQUIRE(fabs(tout[0]) < 1e-6);
    REQUIRE(fabs(tout[9] - 0.9) < 1e-6);
}

TEST_SUITE_END();

#endif
