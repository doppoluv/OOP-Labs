#include "Converters.h"

void TMuteConverter::Convert(IFormat &file, std::vector<std::string> &instruction) const
{
    size_t curOutPos = file.getCurrentOutPos();
    size_t startTime = std::stoull(instruction[instruction.size() - 2]);
    size_t endTime = std::stoull(instruction[instruction.size() - 1]);
    size_t startSample = file.getSampleRate() * startTime;
    size_t endSample = file.getSampleRate() * (endTime - startTime);

    file.setOutPos(sizeof(int16_t) * startSample, std::ios::cur);
    std::cout << "Copying successful" << std::endl;

    for (size_t i = 0; i != endSample; ++i)
    {
        int16_t convertedSample = 0;
        file.sendSample(convertedSample);
    }
    file.setOutPos(curOutPos);
    std::cout << "Converting successful" << std::endl;
}

void TMixConverter::Convert(IFormat &file, std::vector<std::string> &instruction) const
{
    size_t curInPos = file.getCurrentInPos();
    size_t curOutPos = file.getCurrentOutPos();
    size_t startSample = file.getSampleRate() * std::stoull(instruction[2]);
    size_t endSample = file.getSampleRate() * (std::stoull(instruction[3]) - std::stoull(instruction[2]));

    file.setOutPos(sizeof(int16_t) * startSample, std::ios::cur);
    file.setInPos(sizeof(int16_t) * startSample, std::ios::cur);
    std::cout << "Copying successful" << std::endl;

    for (size_t i = 0; i != endSample; ++i)
    {
        int16_t convertedSample = file.getSample() / 2 + file.getExtraSample() / 2;
        file.setOutPos(-sizeof(convertedSample), std::ios::cur);
        file.sendSample(convertedSample);
    }
    file.setInPos(curInPos);
    file.setOutPos(curOutPos);
    std::cout << "Converting successful" << std::endl;
}

void TBassBoostConverter::Convert(IFormat &file, std::vector<std::string> &instruction) const
{
    size_t curOutPos = file.getCurrentOutPos();
    size_t startTime = std::stoull(instruction[instruction.size() - 2]);
    size_t endTime = std::stoull(instruction[instruction.size() - 1]);
    size_t startSample = file.getSampleRate() * startTime;
    size_t endSample = file.getSampleRate() * (endTime - startTime);

    file.setOutPos(sizeof(int16_t) * startSample, std::ios::cur);
    std::cout << "Copying successful" << std::endl;

    double gain = 2.0;
    for (size_t i = 0; i < endSample; ++i)
    {
        int16_t originalSample = file.getSample();
        int16_t boostedSample = static_cast<int16_t>(originalSample * gain);

        if (boostedSample > INT16_MAX)
        {
            boostedSample = INT16_MAX;
        }
        else if (boostedSample < INT16_MIN)
        {
            boostedSample = INT16_MIN;
        }
        file.sendSample(boostedSample);
    }
    file.setOutPos(curOutPos);
    std::cout << "Bass Boost: converting successful" << std::endl;
}