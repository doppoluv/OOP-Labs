#pragma once

#include <iostream>
#include "Sound.h"

class IConverter
{
public:
    virtual void Convert(IFormat &file, std::vector<std::string> &instruction) const = 0;
    virtual ~IConverter() = default;
};

class TMuteConverter : public IConverter
{
public:
    TMuteConverter() = default;
    ~TMuteConverter() override = default;
    void Convert(IFormat &file, std::vector<std::string> &instruction) const override;
};

class TMixConverter : public IConverter
{
public:
    TMixConverter() = default;
    ~TMixConverter() override = default;
    void Convert(IFormat &file, std::vector<std::string> &instruction) const override;
};

class TBassBoostConverter : public IConverter
{
public:
    TBassBoostConverter() = default;
    ~TBassBoostConverter() override = default;
    void Convert(IFormat &file, std::vector<std::string> &instruction) const override;
};