# Config Loader Library

`libconfig_loader` is a C++ library for loading and parsing key-value configuration files. It supports basic data types (strings, integers, doubles, booleans) and vectors of these types.

## Features

*   Load configuration from a text file.
*   Retrieve values by key.
*   **All `get<Type>()` and `getVector<Type>()` functions now come in two versions:**
    *   `get<Type>(key)` / `getVector<Type>(key, delimiter)`: Throws `std::runtime_error` if the key is not found or if the value/element is malformed (for numeric, boolean, or vector element types).
    *   `get<Type>(key, defaultValue)` / `getVector<Type>(key, defaultValue, delimiter)`: Returns the `defaultValue` if the key is not found or the value/element is malformed.
*   Parse comma-separated (or custom delimiter) values into vectors.
*   Control verbosity of error messages (output to `std::cerr` for functions with default values when a default is used due to an error, or when a throwing function would have otherwise printed).
*   Skip empty lines and lines starting with `#` (comments).

## Usage

```cpp
#include "config_loader.h"
```

Create a text file (e.g., `config.txt`) with key-value pairs:

```ini
# Example Configuration
AppName = My Application
Version = 1.2
MaxUsers = 100
EnableFeatureX = true
ServerList = server1.example.com,server2.example.com,192.168.1.100
AdminEmail = admin@example.com
PiValue = 3.14159
DebugMode = FALSE
UserIDs = 10,20,30,40
Thresholds = 0.5;1.5;2.5 # Using a different delimiter
```

In your C++ code, load the configuration file using `Config::loadConfiguration()`:

```cpp
#include "config_loader.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    // Attempt to load the configuration file
    if (!Config::loadConfiguration("config.txt")) {
        std::cerr << "Failed to load configuration file!" << std::endl;
        // Optionally, call Config::setVerboseErrors(true) before loadConfiguration
        // to see detailed error messages from the loader itself if it fails to open the file.
        return 1;
    }

    // Configuration is now loaded and ready to be queried.
    // ...
    return 0;
}
```

Use the `Config::get...()` functions to retrieve values. You can provide a default value that will be returned if the key is not found or if the value cannot be parsed correctly.

**Retrieving Strings:**

```cpp
// Using default value
std::string appName_default = Config::getString("AppName", "DefaultApp");
std::string adminEmail_default = Config::getString("AdminEmail", ""); // Explicitly showing default

// Throws std::runtime_error if "AppName" is missing
try {
    std::string appName_exact = Config::getString("AppName");
    // Use appName_exact
} catch (const std::runtime_error& e) {
    std::cerr << "Configuration error: " << e.what() << std::endl;
}
```

**Retrieving Integers:**

```cpp
// Using default value
int maxUsers_default = Config::getInt("MaxUsers", 50);

// Throws std::runtime_error if "MaxUsers" is missing or malformed
try {
    int maxUsers_exact = Config::getInt("MaxUsers");
    // Use maxUsers_exact
} catch (const std::runtime_error& e) {
    std::cerr << "Configuration error: " << e.what() << std::endl;
}
```

**Retrieving Doubles:**

```cpp
// Using default value if "PiValue" is missing or malformed
double pi_default = Config::getDouble("PiValue", 3.14);

// Throws std::runtime_error if "PiValue" is missing or malformed
try {
    double pi_exact = Config::getDouble("PiValue");
    // Use pi_exact
} catch (const std::runtime_error& e) {
    std::cerr << "Configuration error: " << e.what() << std::endl;
    // Handle error, e.g., exit or use a hardcoded default
}
```

**Retrieving Floats:**
(Similar to `getDouble`, with `float` type)
```cpp
// Using default value
float rate_default = Config::getFloat("Rate", 0.5f);

// Throws std::runtime_error if "Rate" is missing or malformed
try {
    float rate_exact = Config::getFloat("Rate");
    // Use rate_exact
} catch (const std::runtime_error& e) {
    std::cerr << "Configuration error: " << e.what() << std::endl;
}
```

**Retrieving Booleans:**
(Recognizes "true", "false", "1", "0", case-insensitive)

```cpp
// Using default value
bool featureXEnabled_default = Config::getBool("EnableFeatureX", false);
bool debugMode_default = Config::getBool("DebugMode", true);

// Throws std::runtime_error if "EnableFeatureX" is missing or malformed
try {
    bool featureXEnabled_exact = Config::getBool("EnableFeatureX");
    // Use featureXEnabled_exact
} catch (const std::runtime_error& e) {
    std::cerr << "Configuration error: " << e.what() << std::endl;
}
```

**Retrieving Vectors:**
(Default delimiter is comma `,`)
All `getVector...` functions now have a throwing version and a version that accepts a default vector.

```cpp
// Throwing versions
try {
    std::vector<std::string> servers_exact = Config::getVectorString("ServerList");
    // Use servers_exact

    std::vector<int> userIDs_exact = Config::getVectorInt("UserIDs");
    // Use userIDs_exact

    std::vector<double> thresholds_exact = Config::getVectorDouble("Thresholds", ';');
    // Use thresholds_exact

    std::vector<float> factors_exact = Config::getVectorFloat("Factors");
    // Use factors_exact
} catch (const std::runtime_error& e) {
    std::cerr << "Configuration error for exact vector: " << e.what() << std::endl;
}

// Default value versions
std::vector<std::string> default_servers = {"default.server"};
std::vector<std::string> servers_default = Config::getVectorString("NonExistentServerList", default_servers);
// servers_default will be {"default.server"}

std::vector<int> default_ids = {0, -1};
std::vector<int> userIDs_default = Config::getVectorInt("MalformedUserIDs", default_ids);
// If MalformedUserIDs = "10,twenty,30", userIDs_default will be {0, -1}

// Example: If "Factors" was "1.0,abc,2.0", 
// Config::getVectorFloat("Factors") would throw.
// Config::getVectorFloat("Factors", {0.0f}) would return {0.0f}.
// If "Factors" key doesn't exist, Config::getVectorFloat("Factors") would also throw.
```

It is possible to check if a key exists

```cpp
if (Config::hasKey("OptionalSetting")) {
    std::cout << "OptionalSetting is present." << std::endl;
}
```

By default, parsing errors (e.g., trying to get an `int` from "abc") are printed to `std::cerr`. You can disable this:

```cpp
Config::setVerboseErrors(false); // Disable error messages to std::cerr
// ... perform operations ...
Config::setVerboseErrors(true);  // Re-enable if needed
```
This is particularly useful for tests or when you prefer to handle missing/malformed values solely through default values without console output.

## Complete Example

```cpp
#include "config_loader.h"
#include <iostream>
#include <vector>
#include <string>

int main() {

    if (!Config::loadConfiguration("config.txt")) {
        std::cerr << "Critical: Could not load config.txt. Exiting." << std::endl;
        return 1;
    }

    // Using default versions first
    std::string appName = Config::getString("AppName", "Unknown App");
    double version_default = Config::getDouble("Version", 0.0);
    int maxUsers_default = Config::getInt("MaxUsers", 10);
    bool featureX_default = Config::getBool("EnableFeatureX", false);

    std::cout << "Application Name (default): " << appName << std::endl;
    std::cout << "Version (default): " << version_default << std::endl;
    std::cout << "Max Users (default): " << maxUsers_default << std::endl;
    std::cout << "Feature X Enabled (default): " << (featureX_default ? "Yes" : "No") << std::endl;

    // Using throwing versions
    try {
        std::string appName_exact = Config::getString("AppName");
        std::cout << "Application Name (exact): " << appName_exact << std::endl;

        double version_exact = Config::getDouble("Version");
        std::cout << "Version (exact): " << version_exact << std::endl;
        
        int maxUsers_exact = Config::getInt("MaxUsers");
        std::cout << "Max Users (exact): " << maxUsers_exact << std::endl;

        bool featureX_exact = Config::getBool("EnableFeatureX");
        std::cout << "Feature X Enabled (exact): " << (featureX_exact ? "Yes" : "No") << std::endl;
        
        float rate_exact = Config::getFloat("Rate"); // Assuming "Rate = 0.75" in config.txt
        std::cout << "Rate (exact): " << rate_exact << std::endl;

        std::vector<std::string> servers_exact = Config::getVectorString("ServerList");
        std::cout << "Servers (exact):" << std::endl;
        for (const auto& server : servers_exact) {
            std::cout << " - " << server << std::endl;
        }

        std::vector<double> thresholds_exact = Config::getVectorDouble("Thresholds", ';');
        std::cout << "Thresholds (exact):" << std::endl;
        for (double t : thresholds_exact) {
            std::cout << " - " << t << std::endl;
        }
        
        std::vector<float> prices_exact = Config::getVectorFloat("ItemPrices"); // e.g., ItemPrices = 10.5,20.75,5.0
        std::cout << "Item Prices (exact):" << std::endl;
        for (float p : prices_exact) {
            std::cout << " - " << p << std::endl;
        }

    } catch (const std::runtime_error& e) {
        std::cerr << "Error during exact retrieval: " << e.what() << std::endl;
    }

    // Example of vector with default value
    std::vector<int> defaultUserIDs = {-1, -2};
    std::vector<int> userIDs_with_default = Config::getVectorInt("NonExistentUserIDs", defaultUserIDs);
    std::cout << "User IDs (with default for non-existent key):" << std::endl;
    for (int id : userIDs_with_default) {
        std::cout << " - " << id << std::endl;
    }
    // If MalformedUserIDs = "10,twenty,30" in config.txt
    std::vector<int> userIDs_malformed_default = Config::getVectorInt("MalformedUserIDs", defaultUserIDs);
     std::cout << "User IDs (with default for malformed key 'MalformedUserIDs'):" << std::endl;
    for (int id : userIDs_malformed_default) {
        std::cout << " - " << id << std::endl;
    }


    return 0;
}

```

`config.txt` for the example:

```ini
# Example Configuration
AppName = My Application
Version = 1.2
MaxUsers = 100
EnableFeatureX = true
ServerList = server1.example.com,server2.example.com,192.168.1.100
MyNumber = abc # This will cause a parsing error for getInt (uses default)
Thresholds = 0.5;1.5;2.5 # For getVectorDouble
Rate = 0.75 # For getFloat
ItemPrices = 10.5,20.75,5.0 # For getVectorFloat
# MissingKeyDouble will cause getDouble("MissingKeyDouble") to throw
# MalformedVector = 1.0,xyz,3.0 will cause getVectorDouble("MalformedVector") to throw
```
