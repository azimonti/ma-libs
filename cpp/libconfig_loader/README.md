# Config Loader Library

`libconfig_loader` is a C++ library for loading and parsing key-value configuration files. It supports basic data types (strings, integers, doubles, booleans) and vectors of these types.

## Features

*   Load configuration from a text file.
*   Retrieve values by key.
*   Support for default values if a key is not found or the value is malformed.
*   Parse comma-separated (or custom delimiter) values into vectors.
*   Control verbosity of error messages (output to `std::cerr`).
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
std::string appName = Config::getString("AppName", "DefaultApp");
std::string adminEmail = Config::getString("AdminEmail"); // Default is ""
```

**Retrieving Integers:**

```cpp
int maxUsers = Config::getInt("MaxUsers", 50);
```

**Retrieving Doubles:**

```cpp
double pi = Config::getDouble("PiValue", 3.14);
```

**Retrieving Booleans:**
(Recognizes "true", "false", "1", "0", case-insensitive)

```cpp
bool featureXEnabled = Config::getBool("EnableFeatureX", false);
bool debugMode = Config::getBool("DebugMode", true);
```

**Retrieving Vectors:**
(Default delimiter is comma `,`)

```cpp
std::vector<std::string> servers = Config::getVectorString("ServerList");
std::vector<int> userIDs = Config::getVectorInt("UserIDs");

// For vectors with a custom delimiter:
std::vector<double> thresholds = Config::getVectorDouble("Thresholds", ';');
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

    std::string appName = Config::getString("AppName", "Unknown App");
    double version = Config::getDouble("Version", 0.0); // Assuming Version was "1.2"
    int maxUsers = Config::getInt("MaxUsers", 10);
    bool featureX = Config::getBool("EnableFeatureX", false);

    std::cout << "Application Name: " << appName << std::endl;
    std::cout << "Version: " << version << std::endl;
    std::cout << "Max Users: " << maxUsers << std::endl;
    std::cout << "Feature X Enabled: " << (featureX ? "Yes" : "No") << std::endl;

    if (Config::hasKey("ServerList")) {
        std::vector<std::string> servers = Config::getVectorString("ServerList");
        std::cout << "Servers:" << std::endl;
        for (const auto& server : servers) {
            std::cout << " - " << server << std::endl;
        }
    }

    // Example with a non-existent key and default value
    std::string apiKey = Config::getString("API.Key", "DEFAULT_API_KEY");
    std::cout << "API Key: " << apiKey << std::endl;

    // Example with a malformed value (if MyNumber = "abc" in config.txt)
    // If verbose errors are on (default), an error will be printed to std::cerr.
    int myNumber = Config::getInt("MyNumber", 999);
    std::cout << "MyNumber (default if malformed): " << myNumber << std::endl;

    // Example with custom delimiter
    std::vector<double> thresholds = Config::getVectorDouble("Thresholds", ';');
    std::cout << "Thresholds:" << std::endl;
    for (double t : thresholds) {
        std::cout << " - " << t << std::endl;
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
MyNumber = abc # This will cause a parsing error for getInt
Thresholds = 0.5;1.5;2.5
```
