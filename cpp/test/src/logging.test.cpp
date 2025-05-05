#ifndef __clang_analyzer__
/**************************/
/*  logging.test.cpp      */
/*    Version 1.0         */
/*     2022/05/26         */
/**************************/

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include "doctest.h"
#undef ERROR
#include "log/log.h"

TEST_SUITE_BEGIN("[logging]");

TEST_CASE("[logging] Output on stdout")
{
    std::stringstream buffer;
    std::streambuf* coutbuf = std::cout.rdbuf(buffer.rdbuf());
    LOGGER_PARAM(logging::LEVELMAX, logging::INFO);
    LOGGER_PARAM(logging::LOGTIME, true);
    LOGGER(logging::WARNING) << "basic log";
    std::string text = buffer.str();
    std::cout.rdbuf(coutbuf);
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    std::cout << text << std::endl;
    REQUIRE(text.c_str() == doctest::Contains("basic log"));
}

/*
TEST_CASE( "[logging] Testing logging starting with a time" ) {
    std::stringstream buffer;
    std::streambuf *coutbuf = std::cout.rdbuf(buffer.rdbuf());
    LOGGER_PARAM(logging::LEVELMAX, logging::INFO);
    LOGGER_PARAM(logging::LOGTIME, true);
    LOGGER(logging::WARNING) << "log with time";
    std::string text = buffer.str();
    std::cout.rdbuf(coutbuf);
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    std::cout << text << std::endl;
    REQUIRE_THAT( text,  Catch::Matchers::Matches( "\\[\\d*:\\d*:\\d*.*" ) );
}
*/
TEST_CASE("[logging] Testing logging without time")
{
    std::stringstream buffer;
    std::streambuf* coutbuf = std::cout.rdbuf(buffer.rdbuf());
    LOGGER_PARAM(logging::LEVELMAX, logging::INFO);
    LOGGER_PARAM(logging::LOGTIME, false);
    LOGGER(logging::ERROR) << "log without time";
    std::string text = buffer.str();
    std::cout.rdbuf(coutbuf);
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    std::cout << text << std::endl;
    CHECK(text == "[ERROR] log without time");
}

TEST_CASE("[logging] Testing logging level")
{
    std::stringstream buffer;
    std::streambuf* coutbuf = std::cout.rdbuf(buffer.rdbuf());
    LOGGER_PARAM(logging::LEVELMAX, logging::WARNING);
    LOGGER_PARAM(logging::LOGTIME, false);
    LOGGER(logging::INFO) << "low level log - will be skipped";
    LOGGER(logging::ERROR) << "high level log";
    std::string text = buffer.str();
    std::cout.rdbuf(coutbuf);
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    std::cout << text << std::endl;
    CHECK(text == "[ERROR] high level log");
}

TEST_CASE("[logging] Testing logging on file")
{
    std::string fname("test.log");
    std::stringstream buffer;
    LOGGER_PARAM(logging::LEVELMAX, logging::INFO);
    LOGGER_PARAM(logging::LOGTIME, false);
    LOGGER_PARAM(logging::STDOUT, false);
    LOGGER_PARAM(logging::FILENAME, fname.c_str());
    LOGGER_PARAM(logging::FILEOUT, true);
    LOGGER(logging::INFO) << "file log";
    LOGGER_PARAM(logging::FILEOUT, false);
    std::ifstream input_file(fname);
    buffer << input_file.rdbuf();
    input_file.close();
    int deleted      = std::remove(fname.c_str());
    std::string text = buffer.str();
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    std::cout << text << std::endl;
    REQUIRE(deleted == 0);
    CHECK(text == "[INFO] file log");
}

TEST_SUITE_END();

#endif
