#pragma once
#include <string>
#include "ApplicationConfiguration.hpp"

namespace configuration
{
ApplicationConfiguration loadFromIniFile(const std::string& configFilename);
}
