#ifndef __clang_analyzer__
/**************************/
/*  coutext.test.cpp      */
/*    Version 1.0         */
/*     2022/07/20         */
/**************************/

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include "std/cout.h"
#include "doctest.h"

TEST_SUITE_BEGIN("[coutext]");

TEST_CASE("[coutext] Output of a string")
{
    std::stringstream buffer;
    std::streambuf* coutbuf = std::cout.rdbuf(buffer.rdbuf());
    COUT("basic std::cout");
    std::string text = buffer.str();
    std::cout.rdbuf(coutbuf);
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    CHECK(text == "basic std::cout");
}

TEST_CASE("[coutext] Output of an integer")
{
    std::stringstream buffer;
    int i                   = 4;
    std::streambuf* coutbuf = std::cout.rdbuf(buffer.rdbuf());
    COUT(i);
    std::string text = buffer.str();
    std::cout.rdbuf(coutbuf);
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    CHECK(text == "4");
}

TEST_CASE("[coutext] Output of an integer with its name")
{
    std::stringstream buffer;
    int i                   = 4;
    std::streambuf* coutbuf = std::cout.rdbuf(buffer.rdbuf());
    COUTM(i);
    std::string text = buffer.str();
    std::cout.rdbuf(coutbuf);
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    CHECK(text == "i: 4");
}

TEST_CASE("[coutext] Output of a string and an integer")
{
    std::stringstream buffer;
    int i                   = 4;
    std::streambuf* coutbuf = std::cout.rdbuf(buffer.rdbuf());
    COUT("one integer:", i);
    std::string text = buffer.str();
    std::cout.rdbuf(coutbuf);
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    CHECK(text == "one integer: 4");
}

TEST_CASE("[coutext] Output of 32 integers")
{
    std::stringstream buffer;
    std::streambuf* coutbuf = std::cout.rdbuf(buffer.rdbuf());
    COUT(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    std::string text = buffer.str();
    std::cout.rdbuf(coutbuf);
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    CHECK(text == "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15");
}

TEST_CASE("[coutext] Output of 16 integers")
{
    std::stringstream buffer;
    std::streambuf* coutbuf = std::cout.rdbuf(buffer.rdbuf());
    COUTM(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    std::string text = buffer.str();
    std::cout.rdbuf(coutbuf);
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    CHECK(text == "0: 0 # 1: 1 # 2: 2 # 3: 3 # 4: 4 # 5: 5 # 6: 6 # 7: 7 # 8: 8 # 9: 9 # 10: 10 # 11: 11 # 12: 12 # "
                  "13: 13 # 14: 14 # 15: 15");
}

TEST_SUITE_END();

#endif
