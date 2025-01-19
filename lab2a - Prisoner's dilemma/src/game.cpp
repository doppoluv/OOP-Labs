#include "game.h"
#include "strategies.h"
#include "strategyFactory.h"

Game::Game()
{
	initialization();
}

void Game::initialization()
{
	factory.insert("[player]", std::make_unique<Creator<Player>>());

	factory.insert("alwaysd", std::make_unique<Creator<AlwaysDeceiveStrategy>>());
	factory.insert("alwaysc", std::make_unique<Creator<AlwaysCooperateStrategy>>());
	factory.insert("random", std::make_unique<Creator<RandomStrategy>>());
	factory.insert("smart", std::make_unique<Creator<SmartStrategy>>());
	factory.insert("combined", std::make_unique<Creator<CombinedStrategy>>());
}

void Game::start(const struct ParserResult result)
{
	if (result.mode == Mode::Detailed)
	{
		detailed(result.strategies[0], result.strategies[1]);
	}
	else if (result.mode == Mode::Fast)
	{
		fast(result.moves, result.strategies[0], result.strategies[1], result.strategies[2]);
	}
	else if (result.mode == Mode::Tournament)
	{
		tournament(result.moves, result.strategies);
	}
}

void Game::detailed(const std::string &strategy1, const std::string &strategy2)
{
	try
	{
		auto s1 = factory.create(strategy1);
		auto s2 = factory.create(strategy2);
		auto s3 = factory.create("[player]");
		if (s1 == nullptr || s2 == nullptr)
		{
			throw std::invalid_argument("Invalid argyment of strategies");
		}
		std::string input("a");
		while (1)
		{
			int points1_before = points1;
			int points2_before = points2;
			int points3_before = points3;
			char choice1 = s1->run();
			char choice2 = s2->run();
			char userChoice = s3->run();

			if (s3->quitFlag)
			{
				break;
			}

			s1->addHist(choice2, userChoice);
			s2->addHist(choice1, userChoice);
			s3->addHist(choice1, choice2);
			std::string choices = {choice1, choice2, userChoice};
			auto it = transferPoints.find(choices);
			points1 += it->second[0] - '0';
			points2 += it->second[1] - '0';
			points3 += it->second[2] - '0';

			std::cout << "=====================================================" << std::endl;
			std::cout << "Choice for round:" << std::endl;
			std::cout << s1->getName() << " = " << choice1 << " ";
			std::cout << s2->getName() << " = " << choice2 << " ";
			std::cout << s3->getName() << " = " << userChoice << std::endl;
			std::cout << "-----------------------------------------------------" << std::endl;
			std::cout << "Points for round: " << std::endl;
			std::cout << s1->getName() << " = " << points1 - points1_before << " ";
			std::cout << s2->getName() << " = " << points2 - points2_before << " ";
			std::cout << s3->getName() << " = " << points3 - points3_before << std::endl;
			std::cout << "-----------------------------------------------------" << std::endl;
			std::cout << "Points all: " << std::endl;
			std::cout << s1->getName() << " = " << points1 << " ";
			std::cout << s2->getName() << " = " << points2 << " ";
			std::cout << s3->getName() << " = " << points3 << std::endl;
			std::cout << "=====================================================" << std::endl;
		}

		std::cout << "\n=====================================================" << std::endl;
		std::cout << "Game results:" << std::endl;
		std::cout << s1->getName() << " = " << points1 << " ";
		std::cout << s2->getName() << " = " << points2 << " ";
		std::cout << s3->getName() << " = " << points3 << std::endl;
		std::cout << "=====================================================\n"
				  << std::endl;
	}
	catch (std::invalid_argument strategy)
	{
		std::cout << strategy.what() << std::endl;
	}
}

void Game::fast(const int moves, const std::string &strategy1, const std::string &strategy2, const std::string &strategy3)
{
	try
	{
		auto s1 = factory.create(strategy1);
		auto s2 = factory.create(strategy2);
		auto s3 = factory.create(strategy3);

		if (s1 == nullptr || s2 == nullptr || s3 == nullptr)
		{
			throw std::invalid_argument("Invalid argument of strategies");
		}

		for (int i = 0; i < moves; i++)
		{
			char choice1 = s1->run();
			char choice2 = s2->run();
			char choice3 = s3->run();
			s1->addHist(choice2, choice3);
			s2->addHist(choice1, choice3);
			s3->addHist(choice1, choice2);
			std::string choices = {choice1, choice2, choice3};
			auto it = transferPoints.find(choices);
			points1 += it->second[0] - '0';
			points2 += it->second[1] - '0';
			points3 += it->second[2] - '0';
		}

		std::cout << "\n=====================================================" << std::endl;
		std::cout << "Game results:" << std::endl;
		std::cout << s1->getName() << " = " << points1 << " ";
		std::cout << s2->getName() << " = " << points2 << " ";
		std::cout << s3->getName() << " = " << points3 << std::endl;
		std::cout << "=====================================================\n"
				  << std::endl;
	}
	catch (std::invalid_argument strategy)
	{
		std::cout << strategy.what() << std::endl;
	}
}

void Game::tournament(const int moves, const std::vector<std::string> &strategies)
{
	size_t len = strategies.size();
	std::vector<int> stratPoints(len, 0);
	std::string winner;
	int winPoints = 0;
	for (int i = 0; i < len - 2; i++)
	{
		for (int j = i + 1; j < len - 1; j++)
		{
			for (int k = j + 1; k < len; k++)
			{
				auto s1 = strategies[i];
				auto s2 = strategies[j];
				auto s3 = strategies[k];
				points1 = 0;
				points2 = 0;
				points3 = 0;
				fast(moves, s1, s2, s3);
				stratPoints[i] += points1;
				stratPoints[j] += points2;
				stratPoints[k] += points3;
				if (stratPoints[i] > winPoints)
				{
					winPoints = stratPoints[i];
					winner = s1;
				}
				if (stratPoints[j] > winPoints)
				{
					winPoints = stratPoints[j];
					winner = s2;
				}
				if (stratPoints[k] > winPoints)
				{
					winPoints = stratPoints[k];
					winner = s3;
				}
			}
		}
	}

	std::cout << "=====================================================" << std::endl;
	std::cout << "Tournament results:" << std::endl;
	std::cout << "Winner: " << winner << std::endl;
	std::cout << "Points: " << winPoints << std::endl;
	std::cout << "=====================================================\n"
			  << std::endl;
}