#pragma once

#include "WAV.h"

class IFormat
{
public:
    virtual ~IFormat() = default;
    virtual int16_t getSample() const = 0;
    virtual int16_t getExtraSample() const = 0;
    virtual uint32_t getSampleRate() const = 0;
    virtual size_t getLastSample() const = 0;
    virtual void sendSample(const int16_t &sample) = 0;
    virtual size_t getCurrentInPos() = 0;
    virtual size_t getCurrentOutPos() = 0;
    virtual void setInPos(size_t shift) = 0;
    virtual void setInPos(size_t shift, std::ios_base::seekdir dir) = 0;
    virtual void setOutPos(size_t shift) = 0;
    virtual void setOutPos(size_t shift, std::ios_base::seekdir dir) = 0;
};

class TWavFormat : public IFormat
{
public:
    TWavFormat(TWav *fileOut) : out(fileOut) {}
    ~TWavFormat() override = default;
    void Open(TWavReader *file)
    {
        in = file;
    }
    int16_t getSample() const override
    {
        return out->getSample();
    }
    int16_t getExtraSample() const override
    {
        return in->getSample();
    }
    uint32_t getSampleRate() const override
    {
        return out->getHeader().sampleRate;
    }
    size_t getLastSample() const override
    {
        uint32_t dataBlock2Size = out->getHeader().dataBlock2Size;
        uint32_t blockAlign = out->getHeader().blockAlign;
        size_t lastSample = dataBlock2Size / blockAlign;
        return lastSample;
    }
    void sendSample(const int16_t &sample) override
    {
        out->sendSample(sample);
    }
    size_t getCurrentInPos() override
    {
        return in->getCurrentPos();
    }
    size_t getCurrentOutPos() override
    {
        return out->getCurrentPos();
    }
    void setInPos(size_t shift) override
    {
        in->seekg(shift);
    }
    void setInPos(size_t shift, std::ios_base::seekdir dir) override
    {
        in->seekg(shift, dir);
    }
    void setOutPos(size_t shift) override
    {
        out->seekp(shift);
    }
    void setOutPos(size_t shift, std::ios_base::seekdir dir) override
    {
        out->seekp(shift, dir);
    }

private:
    TWavReader *in;
    TWav *out;
};