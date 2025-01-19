#include <iostream>
#include "src/game.h"

int main(int argc, char **argv)
{
	Parsing m;
	Game game;
	auto result = m.parse(argc, argv);
	if (result)
	{
		game.start(*result);
	}

	return 0;
}