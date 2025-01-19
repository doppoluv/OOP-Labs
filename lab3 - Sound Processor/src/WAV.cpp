#include <iostream>
#include "WAV.h"

static const int suppAudioFormat = 1;
static const int suppNumChannels = 1;
static const int suppSampleRate = 44100;
static const int suppBitsPerSample = 16;

TWavReader::TWavReader(const std::string &pathFile)
{
    file.open(pathFile, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + pathFile);
    }

    file.read(reinterpret_cast<char *>(&header.chunkId), sizeof(header.chunkId));
    file.read(reinterpret_cast<char *>(&header.chunkSize), sizeof(header.chunkSize));
    file.read(reinterpret_cast<char *>(&header.format), sizeof(header.format));

    if (std::string(header.chunkId.begin(), header.chunkId.end()) != "RIFF" ||
        std::string(header.format.begin(), header.format.end()) != "WAVE")
    {
        throw std::runtime_error("Invalid WAV file: " + pathFile);
    }

    size_t curPos = file.tellg();
    int count = 0;
    std::string wordFmt = "fmt ";
    std::array<char, 4> temp;
    uint32_t tempSize;

    while (count != 4)
    {
        count = 0;
        file.seekg(curPos, std::ios::beg);
        for (int i = 0; i != header.dataBlock1Id.size(); ++i)
        {
            file.get(temp[i]);
            if (temp[i] == wordFmt[i])
            {
                ++count;
            }
        }
        file.read(reinterpret_cast<char *>(&tempSize), sizeof(tempSize));
        curPos += tempSize + sizeof(tempSize) + sizeof(header.dataBlock1Id);
    }
    header.dataBlock1Id = temp;
    header.dataBlock1Size = tempSize;

    file.read(reinterpret_cast<char *>(&header.audioFormat), sizeof(header.audioFormat));
    if (header.audioFormat != suppAudioFormat)
    {
        throw std::runtime_error("Unsupported audio format in file: " + pathFile);
    }

    file.read(reinterpret_cast<char *>(&header.numChannels), sizeof(header.numChannels));
    if (header.numChannels != suppNumChannels)
    {
        throw std::runtime_error("Unsupported number of channels in file: " + pathFile);
    }

    file.read(reinterpret_cast<char *>(&header.sampleRate), sizeof(header.sampleRate));
    if (header.sampleRate != suppSampleRate)
    {
        throw std::runtime_error("Unsupported sample rate in file: " + pathFile);
    }

    file.read(reinterpret_cast<char *>(&header.byteRate), sizeof(header.byteRate));
    file.read(reinterpret_cast<char *>(&header.blockAlign), sizeof(header.blockAlign));
    file.read(reinterpret_cast<char *>(&header.bitsPerSample), sizeof(header.bitsPerSample));
    if (header.bitsPerSample != suppBitsPerSample)
    {
        throw std::runtime_error("Unsupported bits per sample in file: " + pathFile);
    }

    curPos = file.tellg();
    count = 0;
    std::string wordData = "data";

    while (count != 4)
    {
        count = 0;
        file.seekg(curPos, std::ios::beg);
        for (int i = 0; i != header.dataBlock2Id.size(); ++i)
        {
            file.get(temp[i]);
            if (temp[i] == wordData[i])
            {
                ++count;
            }
        }
        file.read(reinterpret_cast<char *>(&tempSize), sizeof(tempSize));
        curPos += tempSize + sizeof(tempSize) + sizeof(header.dataBlock2Id);
    }
    header.dataBlock2Id = temp;
    header.dataBlock2Size = tempSize;
}

TWavReader::~TWavReader()
{
    if (file.is_open())
    {
        file.close();
    }
}

int16_t TWavReader::getSample()
{
    int16_t sample;
    file.read(reinterpret_cast<char *>(&sample), sizeof(sample));
    return sample;
}

const TWavReader::THeader &TWavReader::getHeader() const
{
    return header;
}

size_t TWavReader::getCurrentPos()
{
    return file.tellg();
}

void TWavReader::seekg(size_t shift)
{
    file.seekg(shift);
}

void TWavReader::seekg(size_t shift, std::ios_base::seekdir dir)
{
    file.seekg(shift, dir);
}

TWav::TWav(const std::string &pathFile)
{
    file.open(pathFile, std::ios::out);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to create file: " + pathFile);
    }
    file.close();

    file.open(pathFile, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + pathFile);
    }
}

TWav::~TWav()
{
    if (file.is_open())
    {
        file.close();
    }
}

void TWav::copyHeader(const TWavReader &otherWav)
{
    header = otherWav.getHeader();
}

void TWav::writeHeader()
{
    file.seekp(0);
    file.write(reinterpret_cast<const char *>(&header), sizeof(header));
}

int16_t TWav::getSample()
{
    int16_t sample;
    file.read(reinterpret_cast<char *>(&sample), sizeof(sample));
    return sample;
}

void TWav::sendSample(const int16_t &sample)
{
    file.write(reinterpret_cast<const char *>(&sample), sizeof(sample));
}

const TWav::THeader &TWav::getHeader() const
{
    return header;
}

size_t TWav::getCurrentPos()
{
    return file.tellp();
}

void TWav::seekp(size_t shift)
{
    file.seekp(shift);
}

void TWav::seekp(size_t shift, std::ios_base::seekdir dir)
{
    file.seekp(shift, dir);
}