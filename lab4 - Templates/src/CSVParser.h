#pragma once

#include <fstream>
#include <string>
#include <tuple>
#include <sstream>
#include <vector>
#include <memory>
#include <exception>
#include <iostream>

enum class Mode
{
    Begin,
    End
};

template <typename... Types>
class CSVParser
{
public:
    CSVParser(std::ifstream &file, int numLinesForSkip, char columnDelim = '\n', char rowDelim = ';', char escapeChar = '"')
        : inputFile(file), numLinesForSkip(numLinesForSkip), columnDelim(columnDelim), rowDelim(rowDelim), escapeChar(escapeChar)
    {
        if (!inputFile.is_open())
            throw std::ios_base::failure("File is not opened");
    }

    std::string getRow()
    {
        std::string row;
        if (!std::getline(inputFile, row, columnDelim))
        {
            throw std::ios_base::failure("End of file reached");
        }
        else
        {
            ++currentLine;
            return row;
        }
    }

    std::vector<std::string> getCells(const std::string &line)
    {
        std::vector<std::string> args;
        std::stringstream stream(line);
        std::string item;
        bool inEscape = false;
        char c;

        while (stream.get(c))
        {
            if (c == escapeChar)
            {
                if (inEscape && stream.peek() == escapeChar)
                {
                    item += escapeChar;
                    stream.get(c);
                }
                else
                {
                    inEscape = !inEscape;
                }
            }
            else if (c == rowDelim && !inEscape)
            {
                args.push_back(item);
                item.clear();
            }
            else
            {
                item += c;
            }
        }
        args.push_back(item);

        return args;
    }

    int getNumOfLine() const
    {
        return currentLine;
    }

    class Iterator;

    Iterator begin()
    {
        Iterator it(*this, Mode::Begin);
        skipLines(it);
        return it;
    }

    Iterator end()
    {
        return Iterator(*this, Mode::End);
    }

private:
    void skipLines(Iterator &it)
    {
        for (int i = 0; i < numLinesForSkip; ++i)
        {
            ++it;
        }
    }

    std::ifstream &inputFile;
    int numLinesForSkip;
    int currentLine = 0;
    char columnDelim;
    char rowDelim;
    char escapeChar;
};

template <typename... Types>
class CSVParser<Types...>::Iterator
{
public:
    using value_type = std::tuple<Types...>;
    using reference = std::tuple<Types...> &;
    using pointer = std::unique_ptr<std::tuple<Types...>>;

    Iterator() = default;
    Iterator(CSVParser<Types...> &parser, Mode mode) : parser(parser)
    {
        if (mode == Mode::Begin)
            updatePointer();
        if (mode == Mode::End)
            ptr = nullptr;
    }

    bool operator!=(const Iterator &other) const
    {
        return this->ptr != other.ptr;
    }

    bool operator==(const Iterator &other) const
    {
        return this->ptr == other.ptr;
    }

    Iterator &operator++()
    {
        updatePointer();
        return *this;
    }

    value_type operator*() const
    {
        return tupleValue;
    }

private:
    value_type tupleValue;
    pointer ptr;
    CSVParser<Types...> &parser;

    template <typename T>
    bool isTypeCorrect(T value, const std::string &cell)
    {
        std::ostringstream os;
        os << value;
        return os.str() == cell;
    }

    void readTuple(std::vector<std::string> &cells, int curPos, int_<sizeof...(Types)>) {}

    template <size_t Size>
    void readTuple(std::vector<std::string> &cells, int curPos, int_<Size>)
    {
        if (Size >= cells.size())
        {
            throw std::runtime_error("Line " + std::to_string(parser.getNumOfLine()) +
                                     ", Cell " + std::to_string(Size + 1) + ": Missing cell data");
        }

        const std::string &cell = cells[Size];
        if (cell.empty())
        {
            throw std::runtime_error("Line " + std::to_string(parser.getNumOfLine()) +
                                     ", Cell " + std::to_string(Size + 1) + ": Empty cell");
        }

        std::tuple_element_t<Size, std::tuple<Types...>> value;
        std::istringstream iss(cell);

        if constexpr (std::is_same_v<std::tuple_element_t<Size, std::tuple<Types...>>, std::string>)
        {
            value = cell;
        }
        else
        {
            iss >> value;
            if (iss.fail() || !iss.eof())
            {
                throw std::runtime_error("Line " + std::to_string(parser.getNumOfLine()) +
                                         ", Cell " + std::to_string(Size + 1) + ": Invalid cell type");
            }
        }

        std::get<Size>(tupleValue) = value;
        readTuple(cells, curPos + 1, int_<Size + 1>());
    }

    void updatePointer()
    {
        try
        {
            std::string line = parser.getRow();
            std::vector<std::string> cells = parser.getCells(line);
            readTuple(cells, 0, int_<0>());
            ptr = std::make_unique<std::tuple<Types...>>(tupleValue);
        }
        catch (const std::ios_base::failure &e)
        {
            ptr = nullptr;
        }
        catch (const std::exception &e)
        {
            ptr = nullptr;
            throw;
        }
    }
};