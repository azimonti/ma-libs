#ifndef _CONFIG_LOADER_H_67815D20DC624671A747829CBD840D02_
#define _CONFIG_LOADER_H_67815D20DC624671A747829CBD840D02_

/*******************/
/* config_loader.h */
/*   Version 1.0   */
/*    2025/05/10   */
/*******************/

#include <optional>
#include <string>
#include <vector>

namespace Config
{
    // Function to load configuration from a file
    // This will populate an internal map.
    bool loadConfiguration(const std::string& filepath);

    std::string getString(const std::string& key);
    std::string getString(const std::string& key, const std::string& defaultValue);
    int getInt(const std::string& key);
    int getInt(const std::string& key, int defaultValue);
    double getDouble(const std::string& key);
    double getDouble(const std::string& key, double defaultValue);
    float getFloat(const std::string& key);
    float getFloat(const std::string& key, float defaultValue);
    bool getBool(const std::string& key);
    bool getBool(const std::string& key, bool defaultValue);

    std::vector<std::string> getVectorString(const std::string& key, char delimiter = ',');
    std::vector<std::string> getVectorString(const std::string& key, const std::vector<std::string>& defaultValue,
                                             char delimiter = ',');
    std::vector<int> getVectorInt(const std::string& key, char delimiter = ',');
    std::vector<int> getVectorInt(const std::string& key, const std::vector<int>& defaultValue, char delimiter = ',');
    std::vector<double> getVectorDouble(const std::string& key, char delimiter = ',');
    std::vector<double> getVectorDouble(const std::string& key, const std::vector<double>& defaultValue,
                                        char delimiter = ',');
    std::vector<float> getVectorFloat(const std::string& key, char delimiter = ',');
    std::vector<float> getVectorFloat(const std::string& key, const std::vector<float>& defaultValue,
                                      char delimiter = ',');

    // Helper to check if a key exists
    bool hasKey(const std::string& key);

    // Function to control verbosity of error messages (to std::cerr)
    // Default is true (verbose errors)
    void setVerboseErrors(bool verbose);

} // namespace Config

#endif
