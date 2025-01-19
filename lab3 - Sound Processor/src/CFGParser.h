#pragma once

#include <fstream>
#include <vector>
#include <string>

class TCFGParser
{
public:
    TCFGParser() = default;
    ~TCFGParser() = default;
    std::vector<std::vector<std::string>> parseConfig(const std::string &filePath);
};