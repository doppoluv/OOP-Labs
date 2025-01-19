#pragma once

#include "game.h"
#include <optional>

enum class Mode
{
	Fast,
	Detailed,
	Tournament,
};

struct ParserResult
{
	std::vector<std::string> strategies;
	Mode mode;
	int moves = 0;
};

class Parsing
{
public:
	std::optional<ParserResult> parse(int argc, char **argv);
};
