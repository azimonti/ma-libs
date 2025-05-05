#ifndef __clang_analyzer__
/**************************/
/*  example.test.cpp      */
/*    Version 1.0         */
/*     2021/07/24         */
/**************************/

#include "doctest.h"

static int Factorial(int number)
{
    return number <= 1 ? number : Factorial(number - 1) * number; // fail
    // return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}

// TEST_CASE( "Factorial of 0 is 1 (fail)", "[example] [!shouldfail]" ) {
//     REQUIRE( Factorial(0) == 1 );
// }

TEST_SUITE_BEGIN("[example]");

TEST_CASE("[example] Factorials of 1 and higher are computed (pass)")
{
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
    // CHECK_THROWS_WITH(FUNCTION, doctest::Contains("ERROR_MESSAGE"));
    // CHECK_THROWS_AS(FUNCTION, std::runtime_error);
}

TEST_SUITE_END();

#endif
