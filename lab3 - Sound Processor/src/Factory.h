#pragma once

#include <memory>
#include <stdexcept>
#include "Converters.h"

class TConverterFactory
{
public:
    std::unique_ptr<IConverter> MakeConverter(std::string_view typeConverter)
    {
        if (typeConverter == "mute")
        {
            return std::make_unique<TMuteConverter>();
        }
        else if (typeConverter == "mix")
        {
            return std::make_unique<TMixConverter>();
        }
        else if (typeConverter == "bassBoost")
        {
            return std::make_unique<TBassBoostConverter>();
        }
        else
        {
            throw std::invalid_argument("Unknown converter type: " + std::string(typeConverter));
        }
    }
};