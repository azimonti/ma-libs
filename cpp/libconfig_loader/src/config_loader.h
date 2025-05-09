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

    // Generic getter functions
    // They will return std::optional to indicate if the key was found and valid
    // Alternatively, they can take a default value and return it if key not found/invalid.
    // For simplicity with the current structure, let's use default values and log errors.

    std::string getString(const std::string& key, const std::string& defaultValue = "");
    int getInt(const std::string& key, int defaultValue = 0);
    double getDouble(const std::string& key, double defaultValue = 0.0);
    bool getBool(const std::string& key, bool defaultValue = false);

    std::vector<std::string> getVectorString(const std::string& key, char delimiter = ',');
    std::vector<int> getVectorInt(const std::string& key, char delimiter = ',');
    std::vector<double> getVectorDouble(const std::string& key, char delimiter = ',');

    // Helper to check if a key exists
    bool hasKey(const std::string& key);

    // Function to control verbosity of error messages (to std::cerr)
    // Default is true (verbose errors)
    void setVerboseErrors(bool verbose);

} // namespace Config

#endif
