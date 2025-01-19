#pragma once

#include "strategies.h"
#include "strategyFactory.h"
#include "parser.h"

class Game
{
public:
	Game();
	void start(const struct ParserResult result);

private:
	Factory factory;
	int points1 = 0;
	int points2 = 0;
	int points3 = 0;
	void initialization();
	void detailed(const std::string &strategy1, const std::string &strategy2);
	void fast(const int moves, const std::string &strategy1, const std::string &strategy2, const std::string &strategy3);
	void tournament(const int moves, const std::vector<std::string> &strategies);
	std::map<std::string, std::string> transferPoints = {{"CCC", "777"},
														 {"CCD", "339"},
														 {"CDC", "393"},
														 {"DCC", "933"},
														 {"CDD", "055"},
														 {"DCD", "505"},
														 {"DDC", "550"},
														 {"DDD", "111"}};
};