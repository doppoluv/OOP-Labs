#include <iostream>
#include <string>
#include <tuple>
#include <fstream>
#include "src/CustomTuple.h"
#include "src/CSVParser.h"

int main() 
{
    std::ifstream file("../test.csv");
    CSVParser<double, int, std::string> parser(file, 0);
    try 
	{
        for (const auto& row : parser) 
		{
            std::cout << std::get<0>(row) << " " << std::get<1>(row) << " " << std::get<2>(row) << std::endl;
        }
    } 
	catch (const std::exception& e) 
	{
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}