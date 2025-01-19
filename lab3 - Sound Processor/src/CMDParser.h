#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "CFGParser.h"
#include "Sound.h"
#include "Factory.h"

class TCMDParser
{
public:
    TCMDParser(const int &argc, char *argv[]);
    ~TCMDParser() = default;
    void convert();

private:
    std::vector<std::unique_ptr<TWavReader>> inputFiles;
    std::unique_ptr<TWav> outputFile;
    std::vector<std::vector<std::string>> configData;
    void copy(TWavFormat &AudioFormat, const std::string &strFileNum);
};