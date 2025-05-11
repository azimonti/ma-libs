/*********************/
/* config_loader.cpp */
/*    Version 1.0    */
/*     2025/05/10    */
/*********************/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include "config_loader.h"

namespace Config
{
    // Internal storage for configuration data
    static std::map<std::string, std::string> s_rawConfig;
    // Control for error message verbosity
    static bool s_verbose_errors = true;

    // Helper function to trim whitespace from both ends of a string
    static std::string trim(const std::string& str)
    {
        size_t first = str.find_first_not_of(" \t\n\r\f\v");
        if (std::string::npos == first) { return str; }
        size_t last = str.find_last_not_of(" \t\n\r\f\v");
        return str.substr(first, (last - first + 1));
    }

    // Helper function to split a string by a delimiter
    static std::vector<std::string> split(const std::string& s, char delimiter)
    {
        std::vector<std::string> tokens;
        if (s.empty()) return tokens; // Handle empty string case
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) { tokens.push_back(trim(token)); }
        return tokens;
    }

    bool loadConfiguration(const std::string& filepath)
    {
        s_rawConfig.clear(); // Clear previous configuration
        std::ifstream configFile(filepath);
        if (!configFile.is_open())
        {
            if (s_verbose_errors) std::cerr << "Error: Could not open configuration file: " << filepath << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(configFile, line))
        {
            line = trim(line);
            if (line.empty() || line[0] == '#')
            { // Skip empty lines and comments
                continue;
            }

            size_t delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos) { continue; }

            std::string key   = trim(line.substr(0, delimiterPos));
            std::string value = trim(line.substr(delimiterPos + 1));

            if (key.empty()) { continue; }
            s_rawConfig[key] = value;
        }
        configFile.close();
        return true;
    }

    bool hasKey(const std::string& key)
    {
        return s_rawConfig.count(key) > 0;
    }

    std::string getString(const std::string& key)
    {
        if (s_rawConfig.count(key)) { return s_rawConfig.at(key); }
        throw std::runtime_error("Config item '" + key + "' was not found.");
    }

    std::string getString(const std::string& key, const std::string& defaultValue)
    {
        try
        {
            return getString(key);
        } catch (const std::runtime_error& e)
        {
            if (s_verbose_errors)
            {
                std::cerr << "Warning: Using default value for string key '" << key << "'. Reason: " << e.what()
                          << std::endl;
            }
            return defaultValue;
        }
    }

    int getInt(const std::string& key)
    {
        if (s_rawConfig.count(key))
        {
            const std::string& val_str = s_rawConfig.at(key);
            try
            {
                size_t processed_chars      = 0;
                int val                     = std::stoi(val_str, &processed_chars);
                std::string trimmed_val_str = trim(val_str);
                if (processed_chars == trimmed_val_str.length()) { return val; }
                else
                {
                    throw std::runtime_error("Config item '" + key +
                                             "' has invalid integer value (extra characters): '" + val_str + "'");
                }
            } catch (const std::invalid_argument& ia)
            {
                throw std::runtime_error("Config item '" + key + "' is not an integer. Value: '" + val_str +
                                         "'. Details: " + ia.what());
            } catch (const std::out_of_range& oor)
            {
                throw std::runtime_error("Config item '" + key + "' is out of range for integer. Value: '" + val_str +
                                         "'. Details: " + oor.what());
            }
        }
        throw std::runtime_error("Config item '" + key + "' was not found.");
    }

    int getInt(const std::string& key, int defaultValue)
    {
        try
        {
            return getInt(key);
        } catch (const std::runtime_error& e)
        {
            if (s_verbose_errors)
            {
                std::cerr << "Warning: Using default value for int key '" << key << "'. Reason: " << e.what()
                          << std::endl;
            }
            return defaultValue;
        }
    }

    double getDouble(const std::string& key)
    {
        if (s_rawConfig.count(key))
        {
            const std::string& val_str = s_rawConfig.at(key);
            try
            {
                size_t processed_chars      = 0;
                double val                  = std::stod(val_str, &processed_chars);
                std::string trimmed_val_str = trim(val_str);
                if (processed_chars == trimmed_val_str.length()) { return val; }
                else
                {
                    throw std::runtime_error("Config item '" + key +
                                             "' has invalid double value (extra characters): '" + val_str + "'");
                }
            } catch (const std::invalid_argument& ia)
            {
                throw std::runtime_error("Config item '" + key + "' is not a double. Value: '" + val_str +
                                         "'. Details: " + ia.what());
            } catch (const std::out_of_range& oor)
            {
                throw std::runtime_error("Config item '" + key + "' is out of range for double. Value: '" + val_str +
                                         "'. Details: " + oor.what());
            }
        }
        throw std::runtime_error("Config item '" + key + "' was not found.");
    }

    double getDouble(const std::string& key, double defaultValue)
    {
        try
        {
            return getDouble(key);
        } catch (const std::runtime_error& e)
        {
            // If verbose errors are on, print the error that would have been thrown
            if (s_verbose_errors)
            {
                std::cerr << "Warning: Using default value for key '" << key << "'. Reason: " << e.what() << std::endl;
            }
            return defaultValue;
        }
    }

    float getFloat(const std::string& key)
    {
        if (s_rawConfig.count(key))
        {
            const std::string& val_str = s_rawConfig.at(key);
            try
            {
                size_t processed_chars      = 0;
                float val                   = std::stof(val_str, &processed_chars);
                std::string trimmed_val_str = trim(val_str);
                if (processed_chars == trimmed_val_str.length()) { return val; }
                else
                {
                    throw std::runtime_error("Config item '" + key + "' has invalid float value (extra characters): '" +
                                             val_str + "'");
                }
            } catch (const std::invalid_argument& ia)
            {
                throw std::runtime_error("Config item '" + key + "' is not a float. Value: '" + val_str +
                                         "'. Details: " + ia.what());
            } catch (const std::out_of_range& oor)
            {
                throw std::runtime_error("Config item '" + key + "' is out of range for float. Value: '" + val_str +
                                         "'. Details: " + oor.what());
            }
        }
        throw std::runtime_error("Config item '" + key + "' was not found.");
    }

    float getFloat(const std::string& key, float defaultValue)
    {
        try
        {
            return getFloat(key);
        } catch (const std::runtime_error& e)
        {
            if (s_verbose_errors)
            {
                std::cerr << "Warning: Using default value for key '" << key << "'. Reason: " << e.what() << std::endl;
            }
            return defaultValue;
        }
    }

    bool getBool(const std::string& key)
    {
        if (s_rawConfig.count(key))
        {
            std::string val_str       = s_rawConfig.at(key);
            std::string lower_val_str = val_str; // Create a mutable copy
            std::transform(lower_val_str.begin(), lower_val_str.end(), lower_val_str.begin(), ::tolower);
            if (lower_val_str == "true" || lower_val_str == "1") { return true; }
            if (lower_val_str == "false" || lower_val_str == "0") { return false; }
            throw std::runtime_error("Config item '" + key + "' has invalid boolean value: '" + val_str +
                                     "'. Expected 'true', 'false', '1', or '0'.");
        }
        throw std::runtime_error("Config item '" + key + "' was not found.");
    }

    bool getBool(const std::string& key, bool defaultValue)
    {
        try
        {
            return getBool(key);
        } catch (const std::runtime_error& e)
        {
            if (s_verbose_errors)
            {
                std::cerr << "Warning: Using default value for bool key '" << key << "'. Reason: " << e.what()
                          << std::endl;
            }
            return defaultValue;
        }
    }

    std::vector<std::string> getVectorString(const std::string& key, char delimiter)
    {
        if (s_rawConfig.count(key)) { return split(s_rawConfig.at(key), delimiter); }
        throw std::runtime_error("Config item (vector string) '" + key + "' was not found.");
    }

    std::vector<std::string> getVectorString(const std::string& key, const std::vector<std::string>& defaultValue,
                                             char delimiter)
    {
        try
        {
            return getVectorString(key, delimiter);
        } catch (const std::runtime_error& e)
        {
            if (s_verbose_errors)
            {
                std::cerr << "Warning: Using default value for vector string key '" << key << "'. Reason: " << e.what()
                          << std::endl;
            }
            return defaultValue;
        }
    }

    std::vector<int> getVectorInt(const std::string& key, char delimiter)
    {
        std::vector<int> vec;
        if (s_rawConfig.count(key))
        {
            std::vector<std::string> parts = split(s_rawConfig.at(key), delimiter);
            for (const auto& p : parts)
            {
                if (p.empty()) continue; // Skip empty parts that might result from e.g. "1,,2"
                try
                {
                    size_t processed_chars = 0;
                    int val                = std::stoi(p, &processed_chars);
                    std::string trimmed_p  = trim(p);
                    if (processed_chars == trimmed_p.length()) { vec.push_back(val); }
                    else
                    {
                        throw std::runtime_error("Invalid integer value (extra characters) '" + p +
                                                 "' in vector for key '" + key + "'.");
                    }
                } catch (const std::invalid_argument& ia)
                {
                    throw std::runtime_error("Invalid integer value '" + p + "' in vector for key '" + key +
                                             "'. Details: " + ia.what());
                } catch (const std::out_of_range& oor)
                {
                    throw std::runtime_error("Integer value '" + p + "' out of range in vector for key '" + key +
                                             "'. Details: " + oor.what());
                }
            }
            return vec;
        }
        throw std::runtime_error("Config item (vector int) '" + key + "' was not found.");
    }

    std::vector<int> getVectorInt(const std::string& key, const std::vector<int>& defaultValue, char delimiter)
    {
        try
        {
            return getVectorInt(key, delimiter);
        } catch (const std::runtime_error& e)
        {
            if (s_verbose_errors)
            {
                std::cerr << "Warning: Using default value for vector int key '" << key << "'. Reason: " << e.what()
                          << std::endl;
            }
            return defaultValue;
        }
    }

    std::vector<double> getVectorDouble(const std::string& key, char delimiter)
    {
        std::vector<double> vec;
        if (s_rawConfig.count(key))
        {
            std::vector<std::string> parts = split(s_rawConfig.at(key), delimiter);
            for (const auto& p : parts)
            {
                if (p.empty()) continue; // Skip empty parts
                try
                {
                    size_t processed_chars = 0;
                    double val             = std::stod(p, &processed_chars);
                    std::string trimmed_p  = trim(p);
                    if (processed_chars == trimmed_p.length()) { vec.push_back(val); }
                    else
                    {
                        // Throw or log, consistent with single value getters.
                        // For now, let's throw, as per user's preference for crashing.
                        // However, for vectors, skipping might be more user-friendly.
                        // Let's stick to throwing for now to be consistent.
                        throw std::runtime_error("Invalid double value (extra characters) '" + p +
                                                 "' in vector for key '" + key + "'.");
                    }
                } catch (const std::invalid_argument& ia)
                {
                    throw std::runtime_error("Invalid double value '" + p + "' in vector for key '" + key +
                                             "'. Details: " + ia.what());
                } catch (const std::out_of_range& oor)
                {
                    throw std::runtime_error("Double value '" + p + "' out of range in vector for key '" + key +
                                             "'. Details: " + oor.what());
                }
            }
            return vec;
        }
        // If key not found, throw, consistent with single value getters that throw.
        // Or return empty vector if that's preferred for "optional" vectors.
        // Given the request for crashing, throwing is more consistent.
        throw std::runtime_error("Config item (vector double) '" + key + "' was not found.");
    }

    std::vector<double> getVectorDouble(const std::string& key, const std::vector<double>& defaultValue, char delimiter)
    {
        try
        {
            return getVectorDouble(key, delimiter);
        } catch (const std::runtime_error& e)
        {
            if (s_verbose_errors)
            {
                std::cerr << "Warning: Using default value for vector double key '" << key << "'. Reason: " << e.what()
                          << std::endl;
            }
            return defaultValue;
        }
    }

    std::vector<float> getVectorFloat(const std::string& key, char delimiter)
    {
        std::vector<float> vec;
        if (s_rawConfig.count(key))
        {
            std::vector<std::string> parts = split(s_rawConfig.at(key), delimiter);
            for (const auto& p : parts)
            {
                if (p.empty()) continue; // Skip empty parts
                try
                {
                    size_t processed_chars = 0;
                    float val              = std::stof(p, &processed_chars);
                    std::string trimmed_p  = trim(p);
                    if (processed_chars == trimmed_p.length()) { vec.push_back(val); }
                    else
                    {
                        throw std::runtime_error("Invalid float value (extra characters) '" + p +
                                                 "' in vector for key '" + key + "'.");
                    }
                } catch (const std::invalid_argument& ia)
                {
                    throw std::runtime_error("Invalid float value '" + p + "' in vector for key '" + key +
                                             "'. Details: " + ia.what());
                } catch (const std::out_of_range& oor)
                {
                    throw std::runtime_error("Float value '" + p + "' out of range in vector for key '" + key +
                                             "'. Details: " + oor.what());
                }
            }
            return vec;
        }
        throw std::runtime_error("Config item (vector float) '" + key + "' was not found.");
    }

    std::vector<float> getVectorFloat(const std::string& key, const std::vector<float>& defaultValue, char delimiter)
    {
        try
        {
            return getVectorFloat(key, delimiter);
        } catch (const std::runtime_error& e)
        {
            if (s_verbose_errors)
            {
                std::cerr << "Warning: Using default value for vector float key '" << key << "'. Reason: " << e.what()
                          << std::endl;
            }
            return defaultValue;
        }
    }

    void setVerboseErrors(bool verbose)
    {
        s_verbose_errors = verbose;
    }

} // namespace Config
