#include "parser.h"

std::optional<ParserResult> Parsing::parse(int argc, char **argv)
{
	ParserResult result;
	if (argc < 4)
	{
		std::cout << "=============================================================================================================" << std::endl;
		std::cout << "Please type: ./Lab2a <strategy1> <strategy2> ... --mode=<...> --moves=<...>" << std::endl;
		std::cout << "-------------------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << "If mode = detailed, the number of strategies should be equal 2" << std::endl;
		std::cout << "If mode = fast, the number of strategies should be equal 3" << std::endl;
		std::cout << "If mode = tournament, the number of strategies should be >= 3" << std::endl;
		std::cout << "-------------------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << "Available strategies: \"alwaysd\", \"alwaysc\", \"random\", \"smart\", \"combined\"." << std::endl;
		std::cout << "=============================================================================================================" << std::endl;
		return std::nullopt;
	}

	int i = 1;
	while (i != argc && argv[i][0] != '-')
	{
		result.strategies.push_back(argv[i]);
		i++;
	}

	if (std::string(argv[i]) == "--mode=detailed")
	{
		if (result.strategies.size() != 2)
		{
			std::cout << "The number of strategies should be equal 2." << std::endl;
			return std::nullopt;
		}
		result.mode = Mode::Detailed;
	}
	else if (std::string(argv[i]) == "--mode=fast")
	{
		if (i + 1 == argc)
		{
			std::cout << "Invalid argument of moves" << std::endl;
			return std::nullopt;
		}
		if (result.strategies.size() != 3)
		{
			std::cout << "The number of strategies should be equal 3." << std::endl;
			return std::nullopt;
		}
		std::string num = std::string(argv[i + 1]).substr(8);
		result.moves = stoi(num);
		result.mode = Mode::Fast;
	}
	else if (std::string(argv[i]) == "--mode=tournament")
	{
		if (i + 1 == argc)
		{
			std::cout << "Invalid argument of moves" << std::endl;
			return std::nullopt;
		}
		if (result.strategies.size() < 3)
		{
			std::cout << "The number of strategies should be >= 3." << std::endl;
			return std::nullopt;
		}
		std::string num = std::string(argv[i + 1]).substr(8);
		result.moves = stoi(num);
		result.mode = Mode::Tournament;
	}
	else
	{
		std::cout << "Invalid argument of mode" << std::endl;
		return std::nullopt;
	}

	return result;
}