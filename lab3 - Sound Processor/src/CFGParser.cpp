#include "CFGParser.h"

std::string trimSpaces(const std::string &input)
{
    std::string result;
    size_t start = input.find_first_not_of(' ');
    size_t end = input.find_last_not_of(' ');

    if (start != std::string::npos && end != std::string::npos)
    {
        result = input.substr(start, end - start + 1);
    }

    return result;
}

std::vector<std::string> splitIntoWords(const std::string &input)
{
    std::vector<std::string> words;
    size_t start = 0;
    size_t end = input.find(' ');

    while (end != std::string::npos)
    {
        std::string word = input.substr(start, end - start);
        if (!word.empty())
        {
            words.push_back(word);
        }
        start = end + 1;
        end = input.find(' ', start);
    }

    std::string lastWord = input.substr(start);
    if (!lastWord.empty())
    {
        words.push_back(lastWord);
    }

    return words;
}

std::vector<std::vector<std::string>> TCFGParser::parseConfig(const std::string &filePath)
{
    std::vector<std::vector<std::string>> configData;
    std::ifstream configFile;
    std::string currentLine;

    configFile.open(filePath, std::ios::in);

    while (std::getline(configFile, currentLine))
    {
        if (currentLine.empty() || currentLine[0] == '#')
        {
            continue;
        }

        std::string trimmedLine = trimSpaces(currentLine);
        std::vector<std::string> words = splitIntoWords(trimmedLine);

        if (!words.empty())
        {
            configData.push_back(words);
        }
    }

    configFile.close();
    return configData;
}