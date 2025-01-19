#include "CMDParser.h"

TCMDParser::TCMDParser(const int &argc, char *argv[])
{
    if (argc < 5)
    {
        throw std::runtime_error("Usage: ./sound_processor -c <config.txt> <output.wav> <input1.wav> [<input2.wav> …]");
    }

    if (std::string(argv[1]) != "-c")
    {
        throw std::runtime_error("Expected '-c' as the second argument.");
    }
    TCFGParser cfgParser;
    configData = cfgParser.parseConfig(argv[2]);

    outputFile = std::make_unique<TWav>(argv[3]);
    for (int i = 4; i < argc; ++i)
    {
        try
        {
            inputFiles.push_back(std::make_unique<TWavReader>(argv[i]));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to create TWavReader for file " << argv[i] << ": " << e.what() << std::endl;
            continue;
        }
    }
}

void TCMDParser::convert()
{
    TConverterFactory factory;
    std::unique_ptr<IConverter> converter;
    TWavFormat format(outputFile.get());
    copy(format, configData[0][1]);

    for (size_t i = 0; i < configData.size(); ++i)
    {
        std::vector<std::string> instruction = configData[i];
        std::cout << "Processing instruction: " << instruction[0] << std::endl;
        converter = factory.MakeConverter(instruction[0]);

        size_t fileNum;
        if (instruction.size() > 3)
        {
            fileNum = std::stoull(instruction[1].erase(0, 1));
            std::cout << "Opening file: " << fileNum << std::endl;
            format.Open(inputFiles[fileNum - 1].get());
        }

        std::cout << "Converting..." << std::endl;
        converter.get()->Convert(format, instruction);
        std::cout << "Conversion done." << std::endl;
    }
}

void TCMDParser::copy(TWavFormat &format, const std::string &strFileNum)
{
    std::string temp = strFileNum;
    size_t fileNum = std::stoull(temp.erase(0, 1));
    outputFile.get()->copyHeader(*(inputFiles[fileNum - 1].get()));
    outputFile.get()->writeHeader();
    format.Open(inputFiles[fileNum - 1].get());

    size_t inPos = inputFiles[fileNum - 1].get()->getCurrentPos();
    size_t outPos = outputFile.get()->getCurrentPos();

    size_t lastSample = format.getLastSample();
    for (size_t i = 0; i < lastSample; ++i)
    {
        int16_t sample = format.getExtraSample();
        format.sendSample(sample);
    }

    inputFiles[fileNum - 1].get()->seekg(inPos);
    outputFile.get()->seekp(outPos);
}