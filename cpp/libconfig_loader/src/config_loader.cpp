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
        std::ifstream configFile(filepath,
                                 std::ios::binary); // std::ios::binary is unusual for text but getline should handle it
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

    std::string getString(const std::string& key, const std::string& defaultValue)
    {
        if (s_rawConfig.count(key)) { return s_rawConfig.at(key); }
        return defaultValue;
    }

    int getInt(const std::string& key, int defaultValue)
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
                    if (s_verbose_errors)
                        std::cerr << "Error: Invalid integer value (extra characters) for key '" << key << "'. Value: '"
                                  << val_str << "'. Using default: " << defaultValue << std::endl;
                }
            } catch (const std::invalid_argument& ia)
            {
                if (s_verbose_errors)
                    std::cerr << "Error: Invalid argument for key '" << key << "'. Value: '" << val_str
                              << "'. Not an integer. " << ia.what() << ". Using default: " << defaultValue << std::endl;
            } catch (const std::out_of_range& oor)
            {
                if (s_verbose_errors)
                    std::cerr << "Error: Out of range for key '" << key << "'. Value: '" << val_str << "'. "
                              << oor.what() << ". Using default: " << defaultValue << std::endl;
            }
        }
        return defaultValue;
    }

    double getDouble(const std::string& key, double defaultValue)
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
                    if (s_verbose_errors)
                        std::cerr << "Error: Invalid double value (extra characters) for key '" << key << "'. Value: '"
                                  << val_str << "'. Using default: " << defaultValue << std::endl;
                }
            } catch (const std::invalid_argument& ia)
            {
                if (s_verbose_errors)
                    std::cerr << "Error: Invalid argument for key '" << key << "'. Value: '" << val_str
                              << "'. Not a double. " << ia.what() << ". Using default: " << defaultValue << std::endl;
            } catch (const std::out_of_range& oor)
            {
                if (s_verbose_errors)
                    std::cerr << "Error: Out of range for key '" << key << "'. Value: '" << val_str << "'. "
                              << oor.what() << ". Using default: " << defaultValue << std::endl;
            }
        }
        return defaultValue;
    }

    bool getBool(const std::string& key, bool defaultValue)
    {
        if (s_rawConfig.count(key))
        {
            std::string val_str = s_rawConfig.at(key);
            std::transform(val_str.begin(), val_str.end(), val_str.begin(), ::tolower);
            if (val_str == "true" || val_str == "1") { return true; }
            if (val_str == "false" || val_str == "0") { return false; }
            if (s_verbose_errors)
                std::cerr << "Error: Invalid boolean value for key '" << key << "'. Value: '" << s_rawConfig.at(key)
                          << "'. Expected 'true', 'false', '1', or '0'. Using default: " << defaultValue << std::endl;
        }
        return defaultValue;
    }

    std::vector<std::string> getVectorString(const std::string& key, char delimiter)
    {
        if (s_rawConfig.count(key)) { return split(s_rawConfig.at(key), delimiter); }
        return {};
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
                    // Check if the entire string was consumed by stoi
                    // Trim p again in case it had internal spaces that trim in split didn't catch, though unlikely with
                    // current trim.
                    std::string trimmed_p  = trim(p);
                    if (processed_chars == trimmed_p.length()) { vec.push_back(val); }
                    else
                    {
                        if (s_verbose_errors)
                            std::cerr << "Error: Invalid integer value (extra characters) '" << p
                                      << "' in vector for key '" << key << "'. Skipping value." << std::endl;
                    }
                } catch (const std::invalid_argument& ia)
                {
                    if (s_verbose_errors)
                        std::cerr << "Error: Invalid integer value '" << p << "' in vector for key '" << key << "'. "
                                  << ia.what() << ". Skipping value." << std::endl;
                } catch (const std::out_of_range& oor)
                {
                    if (s_verbose_errors)
                        std::cerr << "Error: Integer value '" << p << "' out of range in vector for key '" << key
                                  << "'. " << oor.what() << ". Skipping value." << std::endl;
                }
            }
            return vec;
        }
        return {};
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
                        if (s_verbose_errors)
                            std::cerr << "Error: Invalid double value (extra characters) '" << p
                                      << "' in vector for key '" << key << "'. Skipping value." << std::endl;
                    }
                } catch (const std::invalid_argument& ia)
                {
                    if (s_verbose_errors)
                        std::cerr << "Error: Invalid double value '" << p << "' in vector for key '" << key << "'. "
                                  << ia.what() << ". Skipping value." << std::endl;
                } catch (const std::out_of_range& oor)
                {
                    if (s_verbose_errors)
                        std::cerr << "Error: Double value '" << p << "' out of range in vector for key '" << key
                                  << "'. " << oor.what() << ". Skipping value." << std::endl;
                }
            }
            return vec;
        }
        return {};
    }

    void setVerboseErrors(bool verbose)
    {
        s_verbose_errors = verbose;
    }

} // namespace Config
