#ifndef __clang_analyzer__
/**************************/
/* config_loader.test.cpp */
/*    Version 1.0         */
/*     2025/05/10         */
/**************************/

#include <cstdio>
#include <string>
#include <vector>
#include "config_loader.h"
#include "doctest.h"

TEST_SUITE_BEGIN("[config_loader]");

// Fixture to load configuration
struct ConfigFixture
{
    ConfigFixture()
    {
        bool loaded = Config::loadConfiguration("cpp/test/data/config_loader.txt");
        REQUIRE(loaded == true);
        Config::setVerboseErrors(false); // Disable std::cerr output for tests
    }

    ~ConfigFixture()
    {
        Config::setVerboseErrors(true); // Optional: restore default verbosity after tests
    }
};

TEST_CASE_FIXTURE(ConfigFixture, "[config_loader] Vector Value Retrieval from Focused Config")
{
    SUBCASE("GetVectorString - TestData.StringVector")
    {
        std::vector<std::string> expected_str_vec = {"alpha", "beta", "gamma"};
        CHECK(Config::getVectorString("TestData.StringVector") == expected_str_vec);
    }

    SUBCASE("GetVectorString - Test.VectorWithEmptyParts")
    {
        std::vector<std::string> expected_empty_parts = {"val1", "", "val2"};
        CHECK(Config::getVectorString("Test.VectorWithEmptyParts") == expected_empty_parts);
    }

    SUBCASE("GetVectorString - Test.EmptyVector")
    {
        // Depending on `split` behavior for an empty string on the RHS of '=',
        // it might return a vector with one empty string, or a completely empty vector.
        // The current `split` in config_loader.cpp returns an empty vector if the input string `s` is empty.
        // If "Test.EmptyVector = " results in s_rawConfig["Test.EmptyVector"] being "", then an empty vector is
        // correct.
        std::vector<std::string> actual_empty_vector = Config::getVectorString("Test.EmptyVector");
        CHECK(actual_empty_vector.empty() == true);
    }

    SUBCASE("GetVectorString - NonExistent Key")
    {
        std::vector<std::string> empty_vec_str;
        CHECK(Config::getVectorString("NonExistent.KeyStringVec") == empty_vec_str);
    }
}

TEST_CASE_FIXTURE(ConfigFixture, "[config_loader] Malformed Vector Retrieval from Focused Config")
{
    SUBCASE("MalformedVectorInt - Test.MalformedVectorInt")
    {
        // Test.MalformedVectorInt = 10,twenty,30,40.5
        // "twenty" and "40.5" should be skipped, errors logged.
        std::vector<int> expected_malformed_int = {10, 30};
        CHECK(Config::getVectorInt("Test.MalformedVectorInt") == expected_malformed_int);
    }

    SUBCASE("MalformedVectorDouble - Test.MalformedVectorDouble")
    {
        // Test.MalformedVectorDouble = 10.1,thirty-point-three,30.3
        // "thirty-point-three" should be skipped, errors logged.
        std::vector<double> expected_malformed_double = {10.1, 30.3};
        std::vector<double> actual_malformed_double   = Config::getVectorDouble("Test.MalformedVectorDouble");
        REQUIRE(actual_malformed_double.size() == 2);
        CHECK(actual_malformed_double[0] == doctest::Approx(10.1));
        CHECK(actual_malformed_double[1] == doctest::Approx(30.3));
    }
}

TEST_CASE_FIXTURE(ConfigFixture, "[config_loader] Basic Value Retrieval - Default Values")
{
    SUBCASE("GetString - NonExistent Key")
    {
        CHECK(Config::getString("NonExistent.StringKey", "default_string") == "default_string");
    }

    SUBCASE("GetInt - NonExistent Key")
    {
        CHECK(Config::getInt("NonExistent.IntKey", 999) == 999);
    }

    SUBCASE("GetDouble - NonExistent Key")
    {
        CHECK(Config::getDouble("NonExistent.DoubleKey", 1.2345) == doctest::Approx(1.2345));
    }

    SUBCASE("GetBool - NonExistent Key")
    {
        CHECK(Config::getBool("NonExistent.BoolKeyTrue", true) == true);
        CHECK(Config::getBool("NonExistent.BoolKeyFalse", false) == false);
    }
}

TEST_CASE("[config_loader] File Handling")
{
    Config::setVerboseErrors(false); // Suppress std::cerr for this test case
    SUBCASE("Load Non-Existent File")
    {
        bool loaded = Config::loadConfiguration("this_file_should_not_exist.txt");
        CHECK(loaded == false);
        // After a failed load, s_rawConfig should be empty or in a predictable state.
        // The current implementation clears s_rawConfig at the start of loadConfiguration.
        CHECK(Config::getString("AnyKeyWhatsoever", "default_after_failed_load") == "default_after_failed_load");
        CHECK(Config::hasKey("TestData.StringVector") == false); // Should not retain old data
    }
    Config::setVerboseErrors(true); // Restore default verbosity
}

TEST_CASE_FIXTURE(ConfigFixture, "[config_loader] Extended Value Retrieval and Edge Cases")
{
    SUBCASE("Basic String Retrieval - Test.String")
    {
        CHECK(Config::getString("Test.String") == "Hello World");
    }
    SUBCASE("Basic Int Retrieval - Test.Int")
    {
        CHECK(Config::getInt("Test.Int") == 123);
    }
    SUBCASE("Basic Double Retrieval - Test.Double")
    {
        CHECK(Config::getDouble("Test.Double") == doctest::Approx(45.67));
    }
    SUBCASE("Boolean Retrievals - True Variants")
    {
        CHECK(Config::getBool("Test.BoolTrue") == true);
        CHECK(Config::getBool("Test.BoolOne") == true);
        CHECK(Config::getBool("Test.BoolCapsTrue") == true);
    }
    SUBCASE("Boolean Retrievals - False Variants")
    {
        CHECK(Config::getBool("Test.BoolFalse") == false);
        CHECK(Config::getBool("Test.BoolZero") == false);
        CHECK(Config::getBool("Test.BoolCapsFalse") == false);
    }

    SUBCASE("Whitespace Handling - String")
    {
        CHECK(Config::getString("Test.StringWithSpaces") == "leading and trailing spaces");
    }
    SUBCASE("Whitespace Handling - Int")
    {
        CHECK(Config::getInt("Test.IntWithSpaces") == 234);
    }
    SUBCASE("Whitespace Handling - Double")
    {
        CHECK(Config::getDouble("Test.DoubleWithSpaces") == doctest::Approx(56.78));
    }

    SUBCASE("Negative Number Retrieval - Int")
    {
        CHECK(Config::getInt("Test.NegativeInt") == -100);
    }
    SUBCASE("Negative Number Retrieval - Double")
    {
        CHECK(Config::getDouble("Test.NegativeDouble") == doctest::Approx(-99.88));
    }

    SUBCASE("Vector with Different Delimiter - Semicolon")
    {
        std::vector<std::string> expected_vec = {"one", "two", "three"};
        CHECK(Config::getVectorString("Test.VectorWithSemicolon", ';') == expected_vec);
    }

    SUBCASE("Empty Value Handling - String")
    {
        CHECK(Config::getString("Test.EmptyStringValue", "default_for_empty") ==
              ""); // Empty value should be an empty string
    }
    SUBCASE("Empty Value Handling - Int (should use default)")
    {
        CHECK(Config::getInt("Test.EmptyIntValue", 777) == 777);
    }
    SUBCASE("Empty Value Handling - Double (should use default)")
    {
        CHECK(Config::getDouble("Test.EmptyDoubleValue", 88.88) == doctest::Approx(88.88));
    }
    SUBCASE("Empty Value Handling - Bool (should use default)")
    {
        CHECK(Config::getBool("Test.EmptyBoolValue", true) == true);
        CHECK(Config::getBool("Test.EmptyBoolValue", false) == false);
    }

    SUBCASE("Invalid Boolean Value (should use default)")
    {
        CHECK(Config::getBool("Test.InvalidBool", true) == true);
        CHECK(Config::getBool("Test.InvalidBool", false) == false);
    }
}

TEST_SUITE_END();

#endif // __clang_analyzer__
