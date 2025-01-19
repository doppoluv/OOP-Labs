#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <fstream>

class IWavFile
{
public:
    virtual ~IWavFile() = default;

protected:
    struct THeader
    {
        std::array<char, 4> chunkId;      // "RIFF"
        uint32_t chunkSize;               // Размер файла (size - 8)
        std::array<char, 4> format;       // "WAVE"
        std::array<char, 4> dataBlock1Id; // "fmt "
        uint32_t dataBlock1Size;          // Размер подструктуры
        int16_t audioFormat;              // Аудиоформат (PCM = 1)
        int16_t numChannels;              // Количество каналов
        uint32_t sampleRate;              // Частота дискретизации
        uint32_t byteRate;                // Количество байт в секунду
        int16_t blockAlign;               // Количество байт в сэмпле
        int16_t bitsPerSample;            // Количество бит в сэмпле
        std::array<char, 4> dataBlock2Id; // "data"
        uint32_t dataBlock2Size;          // Количество байт в области данных
    };
    std::string pathFile;
    THeader header;
};

class TWavReader : public IWavFile
{
public:
    TWavReader() = default;
    TWavReader(const std::string &pathFile);
    ~TWavReader() override;

    int16_t getSample();
    const THeader &getHeader() const;
    size_t getCurrentPos();
    void seekg(size_t shift);
    void seekg(size_t shift, std::ios_base::seekdir dir);

private:
    std::ifstream file;
};

class TWav : public IWavFile
{
public:
    TWav() = default;
    TWav(const std::string &pathFile);
    ~TWav() override;

    void copyHeader(const TWavReader &otherWav);
    void writeHeader();
    int16_t getSample();
    void sendSample(const int16_t &sample);
    const THeader &getHeader() const;
    size_t getCurrentPos();
    void seekp(size_t shift);
    void seekp(size_t shift, std::ios_base::seekdir dir);

private:
    std::fstream file;
};
