#ifndef __clang_analyzer__
/*****************************/
/*  logging_release.test.cpp */
/*    Version 1.0            */
/*     2022/05/26            */
/*****************************/

#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include "doctest.h"
#undef ERROR
#include "log/log.h"

std::string long_function_sleeping(size_t i)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<unsigned int>(i) * 1000));
    return std::string("string after " + std::to_string(i) + " second sleep");
}

#if defined(LOGGING)

TEST_SUITE_BEGIN("[logging]");

TEST_CASE("[logging] No output on stdout")
{
    std::stringstream buffer;
    std::streambuf* coutbuf = std::cout.rdbuf(buffer.rdbuf());
    LOGGER_PARAM(logging::LEVELMAX, logging::INFO);
    LOGGER_PARAM(logging::LOGTIME, true);
    LOGGER(logging::WARNING) << "test log to be seen";
    std::string text = buffer.str();
    std::cout.rdbuf(coutbuf);
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    std::cout << text << std::endl;
    REQUIRE(text.c_str() == doctest::Contains("test log"));
}

TEST_CASE("[logging] Long function should not be output")
{
    const size_t delay = 5;
    auto t0            = std::chrono::high_resolution_clock::now();
    LOGGER_PARAM(logging::LEVELMAX, logging::INFO);
    LOGGER_PARAM(logging::LOGTIME, true);
    LOGGER(logging::WARNING) << std::string("start test logging a function with " + std::to_string(delay) +
                                            " seconds delay");
    LOGGER(logging::WARNING) << long_function_sleeping(delay);
    auto t1                         = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> fs = t1 - t0;
    std::chrono::milliseconds d     = std::chrono::duration_cast<std::chrono::milliseconds>(fs);
    CHECK(static_cast<unsigned int>(d.count()) > static_cast<unsigned int>(delay - 1));
}

#else

TEST_CASE("[logging] Output on stdout")
{
    std::stringstream buffer;
    std::streambuf* coutbuf = std::cout.rdbuf(buffer.rdbuf());
    LOGGER_PARAM(logging::LEVELMAX, logging::INFO);
    LOGGER_PARAM(logging::LOGTIME, true);
    LOGGER(logging::WARNING) << "test log if seen is wrong";
    std::string text = buffer.str();
    std::cout.rdbuf(coutbuf);
    std::cout << text << std::endl;
    CHECK(text == "");
}

TEST_CASE("[logging] Long function should output")
{
    const size_t delay = 5;
    auto t0            = std::chrono::high_resolution_clock::now();
    LOGGER_PARAM(logging::LEVELMAX, logging::INFO);
    LOGGER_PARAM(logging::LOGTIME, true);
    LOGGER(logging::WARNING) << std::string("start test logging a function with " + std::to_string(delay) +
                                            " seconds delay");
    LOGGER(logging::WARNING) << long_function_sleeping(delay);
    auto t1                         = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> fs = t1 - t0;
    std::chrono::milliseconds d     = std::chrono::duration_cast<std::chrono::milliseconds>(fs);
    std::cout << d.count() << "s\n";
    CHECK(static_cast<unsigned int>(d.count()) < static_cast<unsigned int>(delay - 1));
}

TEST_SUITE_END();

#endif
#endif
