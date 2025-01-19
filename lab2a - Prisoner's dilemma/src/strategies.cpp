#include "game.h"
#include "strategies.h"
#include "strategyFactory.h"

#include <iostream>
#include <string>

char Player::run()
{
    std::cout << "Press \"C\" or \"D\" or \"quit\"" << std::endl;
    while (1)
    {
        std::string userChoiceInput;
        std::cin >> userChoiceInput;
        char userChoice;
        if (userChoiceInput == "quit")
        {
            quitFlag = true;
            return 'C';
        }
        if (userChoiceInput == "C")
        {
            return 'C';
        }
        else if (userChoiceInput == "D")
        {
            return 'D';
        }

        std::cout << "!ERROR! \nPress \"C\" or \"D\" or \"quit\"" << std::endl;
    }
}

const std::string &Player::getName() const
{
    return name;
}

char AlwaysDeceiveStrategy::run()
{
    return 'D';
}

const std::string &AlwaysDeceiveStrategy::getName() const
{
    return name;
}

char AlwaysCooperateStrategy::run()
{
    return 'C';
}

const std::string &AlwaysCooperateStrategy::getName() const
{
    return name;
}

char RandomStrategy::run()
{
    srand(time(0));
    return rand() % 2 ? 'C' : 'D';
}

const std::string &RandomStrategy::getName() const
{
    return name;
}

char SmartStrategy::run()
{
    if (m_s2 == 'C' && m_s3 == 'C')
    {
        return 'C';
    }
    else
    {
        return 'D';
    }
}

void SmartStrategy::addHist(char s2, char s3)
{
    m_s2 = s2;
    m_s3 = s3;
}

const std::string &SmartStrategy::getName() const
{
    return name;
}

char CombinedStrategy::run()
{
    if (countD > countC)
    {
        return 'D';
    }
    else if (countD = countC)
    {
        srand(time(0));
        return rand() % 2 ? 'C' : 'D';
    }
    else
    {
        return 'C';
    }
}

void CombinedStrategy::addHist(char s2, char s3)
{
    if (s2 == 'C')
    {
        countC++;
    }
    else
    {
        countD++;
    }

    if (s3 == 'C')
    {
        countC++;
    }
    else
    {
        countD++;
    }
}

const std::string &CombinedStrategy::getName() const
{
    return name;
}