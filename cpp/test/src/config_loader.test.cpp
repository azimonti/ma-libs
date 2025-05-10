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
}

TEST_CASE_FIXTURE(ConfigFixture, "[config_loader] Malformed Vector Retrieval from Focused Config")
{
    SUBCASE("MalformedVectorInt - Test.MalformedVectorInt")
    {
        // Test.MalformedVectorInt = 10,twenty,30,40.5
        // "twenty" or "40.5" should cause a throw.
        CHECK_THROWS_AS(Config::getVectorInt("Test.MalformedVectorInt"), std::runtime_error);
    }
}

TEST_CASE_FIXTURE(ConfigFixture, "[config_loader] Throwing Behavior Tests")
{
    SUBCASE("GetString - Successful Retrieval")
    {
        CHECK(Config::getString("Test.String") == "Hello World");
    }
    SUBCASE("GetString - NonExistent Key Throws")
    {
        CHECK_THROWS_AS(Config::getString("NonExistent.StringKey.Throw"), std::runtime_error);
    }

    SUBCASE("GetInt - Successful Retrieval")
    {
        CHECK(Config::getInt("Test.Int") == 123);
    }
    SUBCASE("GetInt - NonExistent Key Throws")
    {
        CHECK_THROWS_AS(Config::getInt("NonExistent.IntKey.Throw"), std::runtime_error);
    }
    SUBCASE("GetInt - Malformed Value Throws")
    {
        // Test.MalformedIntForThrow = 123xyz
        CHECK_THROWS_AS(Config::getInt("Test.MalformedIntForThrow"), std::runtime_error);
    }
    SUBCASE("GetInt - Value with Extra Chars Throws")
    {
        // Test.IntWithExtraForThrow = 789 jkl
        CHECK_THROWS_AS(Config::getInt("Test.IntWithExtraForThrow"), std::runtime_error);
    }

    SUBCASE("GetDouble - Successful Retrieval")
    {
        CHECK(Config::getDouble("Test.Double") == doctest::Approx(45.67));
    }
    SUBCASE("GetDouble - NonExistent Key Throws")
    {
        CHECK_THROWS_AS(Config::getDouble("NonExistent.DoubleKey.Throw"), std::runtime_error);
    }
    SUBCASE("GetDouble - Malformed Value Throws")
    {
        // Test.MalformedDouble = not_a_double
        CHECK_THROWS_AS(Config::getDouble("Test.MalformedDouble"), std::runtime_error);
    }
    SUBCASE("GetDouble - Value with Extra Chars Throws")
    {
        // Test.DoubleWithExtra = 7.89 jkl
        CHECK_THROWS_AS(Config::getDouble("Test.DoubleWithExtra"), std::runtime_error);
    }

    SUBCASE("GetFloat - Successful Retrieval")
    {
        // Test.Float = 1.23
        CHECK(Config::getFloat("Test.Float") == doctest::Approx(1.23f));
    }
    SUBCASE("GetFloat - NonExistent Key Throws")
    {
        CHECK_THROWS_AS(Config::getFloat("NonExistent.FloatKey.Throw"), std::runtime_error);
    }
    SUBCASE("GetFloat - Malformed Value Throws")
    {
        // Test.MalformedFloat = abc
        CHECK_THROWS_AS(Config::getFloat("Test.MalformedFloat"), std::runtime_error);
    }
    SUBCASE("GetFloat - Value with Extra Chars Throws")
    {
        // Test.FloatWithExtra = 4.56 ghi
        CHECK_THROWS_AS(Config::getFloat("Test.FloatWithExtra"), std::runtime_error);
    }

    SUBCASE("GetBool - Successful Retrieval - True")
    {
        CHECK(Config::getBool("Test.BoolTrue") == true);
    }
    SUBCASE("GetBool - Successful Retrieval - False")
    {
        CHECK(Config::getBool("Test.BoolFalse") == false);
    }
    SUBCASE("GetBool - NonExistent Key Throws")
    {
        CHECK_THROWS_AS(Config::getBool("NonExistent.BoolKey.Throw"), std::runtime_error);
    }
    SUBCASE("GetBool - Malformed Value Throws")
    {
        // Test.MalformedBoolForThrow = maybe
        CHECK_THROWS_AS(Config::getBool("Test.MalformedBoolForThrow"), std::runtime_error);
    }

    SUBCASE("GetVectorString - Successful Retrieval")
    {
        std::vector<std::string> expected_str_vec = {"alpha", "beta", "gamma"};
        CHECK(Config::getVectorString("TestData.StringVector") == expected_str_vec);
    }
    SUBCASE("GetVectorString - NonExistent Key Throws")
    {
        CHECK_THROWS_AS(Config::getVectorString("NonExistent.KeyStringVec.Throw"), std::runtime_error);
    }

    SUBCASE("GetVectorInt - Successful Retrieval")
    {
        // Test.GoodVectorInt = 1,2,3,4
        std::vector<int> expected_vec = {1, 2, 3, 4};
        std::vector<int> actual_vec   = Config::getVectorInt("Test.GoodVectorInt");
        CHECK(actual_vec == expected_vec);
    }
    SUBCASE("GetVectorInt - NonExistent Key Throws")
    {
        CHECK_THROWS_AS(Config::getVectorInt("NonExistent.VectorIntKey.Throw"), std::runtime_error);
    }
    SUBCASE("GetVectorInt - Malformed Element Throws") // This is covered by the updated MalformedVectorInt test case
    {
        // Test.MalformedVectorInt = 10,twenty,30,40.5
        CHECK_THROWS_AS(Config::getVectorInt("Test.MalformedVectorInt"), std::runtime_error);
    }

    SUBCASE("GetVectorDouble - Successful Retrieval")
    {
        // Test.GoodVectorDouble = 10.1,20.2,30.3
        std::vector<double> expected_vec = {10.1, 20.2, 30.3};
        std::vector<double> actual_vec   = Config::getVectorDouble("Test.GoodVectorDouble");
        REQUIRE(actual_vec.size() == expected_vec.size());
        for (size_t i = 0; i < actual_vec.size(); ++i) { CHECK(actual_vec[i] == doctest::Approx(expected_vec[i])); }
    }
    SUBCASE("GetVectorDouble - NonExistent Key Throws")
    {
        CHECK_THROWS_AS(Config::getVectorDouble("NonExistent.VectorDoubleKey.Throw"), std::runtime_error);
    }
    SUBCASE("GetVectorDouble - Malformed Element Throws")
    {
        // Test.MalformedVectorDouble = 10.1,thirty-point-three,30.3
        CHECK_THROWS_AS(Config::getVectorDouble("Test.MalformedVectorDouble"), std::runtime_error);
    }

    SUBCASE("GetVectorFloat - Successful Retrieval")
    {
        // Test.VectorFloat = 1.1,2.2,3.3
        std::vector<float> expected_vec = {1.1f, 2.2f, 3.3f};
        std::vector<float> actual_vec   = Config::getVectorFloat("Test.VectorFloat");
        REQUIRE(actual_vec.size() == expected_vec.size());
        for (size_t i = 0; i < actual_vec.size(); ++i) { CHECK(actual_vec[i] == doctest::Approx(expected_vec[i])); }
    }
    SUBCASE("GetVectorFloat - NonExistent Key Throws")
    {
        CHECK_THROWS_AS(Config::getVectorFloat("NonExistent.VectorFloatKey.Throw"), std::runtime_error);
    }
    SUBCASE("GetVectorFloat - Malformed Element Throws")
    {
        // Test.MalformedVectorFloat = 1.1,xyz,3.3
        CHECK_THROWS_AS(Config::getVectorFloat("Test.MalformedVectorFloat"), std::runtime_error);
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

TEST_CASE_FIXTURE(ConfigFixture, "[config_loader] Vector Value Retrieval - Default Values")
{
    SUBCASE("GetVectorString - Default for NonExistent Key")
    {
        std::vector<std::string> default_val = {"default", "vector"};
        CHECK(Config::getVectorString("NonExistent.VectorStringKey", default_val) == default_val);
    }
    // For GetVectorString, malformed elements are not really a concept as all parts are strings.

    SUBCASE("GetVectorInt - Default for NonExistent Key")
    {
        std::vector<int> default_val = {11, 22};
        CHECK(Config::getVectorInt("NonExistent.VectorIntKey", default_val) == default_val);
    }
    SUBCASE("GetVectorInt - Default for Malformed Key")
    {
        // Test.MalformedVectorInt = 10,twenty,30,40.5
        std::vector<int> default_val = {99, 88};
        CHECK(Config::getVectorInt("Test.MalformedVectorInt", default_val) == default_val);
    }

    SUBCASE("GetVectorDouble - Default for NonExistent Key")
    {
        std::vector<double> default_val = {1.11, 2.22};
        std::vector<double> result      = Config::getVectorDouble("NonExistent.VectorDoubleKey", default_val);
        REQUIRE(result.size() == default_val.size());
        for (size_t i = 0; i < result.size(); ++i) CHECK(result[i] == doctest::Approx(default_val[i]));
    }
    SUBCASE("GetVectorDouble - Default for Malformed Key")
    {
        // Test.MalformedVectorDouble = 10.1,thirty-point-three,30.3
        std::vector<double> default_val = {9.99, 8.88};
        std::vector<double> result      = Config::getVectorDouble("Test.MalformedVectorDouble", default_val);
        REQUIRE(result.size() == default_val.size());
        for (size_t i = 0; i < result.size(); ++i) CHECK(result[i] == doctest::Approx(default_val[i]));
    }

    SUBCASE("GetVectorFloat - Default for NonExistent Key")
    {
        std::vector<float> default_val = {1.1f, 2.2f};
        std::vector<float> result      = Config::getVectorFloat("NonExistent.VectorFloatKey", default_val);
        REQUIRE(result.size() == default_val.size());
        for (size_t i = 0; i < result.size(); ++i) CHECK(result[i] == doctest::Approx(default_val[i]));
    }
    SUBCASE("GetVectorFloat - Default for Malformed Key")
    {
        // Test.MalformedVectorFloat = 1.1,xyz,3.3
        std::vector<float> default_val = {9.9f, 8.8f};
        std::vector<float> result      = Config::getVectorFloat("Test.MalformedVectorFloat", default_val);
        REQUIRE(result.size() == default_val.size());
        for (size_t i = 0; i < result.size(); ++i) CHECK(result[i] == doctest::Approx(default_val[i]));
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
