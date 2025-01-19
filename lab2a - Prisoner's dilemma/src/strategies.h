#pragma once

#include <iostream>
#include <string>
#include <vector>

class StrategyInterface
{
public:
	virtual ~StrategyInterface() {};
	virtual char run() = 0;
	virtual void addHist(char s2, char s3) = 0;
	virtual const std::string &getName() const = 0;
	bool quitFlag = false;
};

class Player : public StrategyInterface
{
public:
	~Player() override = default;
	char run() override;
	void addHist(char s2, char s3) override {}
	const std::string &getName() const override;

private:
	const std::string name = "[player]";
};

class AlwaysDeceiveStrategy : public StrategyInterface
{
public:
	~AlwaysDeceiveStrategy() override = default;
	char run() override;
	void addHist(char s2, char s3) override {}
	const std::string &getName() const override;

private:
	const std::string name = "alwaysd";
};

class AlwaysCooperateStrategy : public StrategyInterface
{
public:
	~AlwaysCooperateStrategy() override = default;
	char run() override;
	void addHist(char s2, char s3) override {}
	const std::string &getName() const override;

private:
	const std::string name = "alwaysc";
};

class RandomStrategy : public StrategyInterface
{
public:
	~RandomStrategy() override = default;
	char run() override;
	void addHist(char s2, char s3) override {}
	const std::string &getName() const override;

private:
	const std::string name = "random";
};

class SmartStrategy : public StrategyInterface
{
public:
	~SmartStrategy() override = default;
	char run() override;
	void addHist(char s2, char s3) override;
	const std::string &getName() const override;

private:
	const std::string name = "smart";
	char m_s2;
	char m_s3;
};

class CombinedStrategy : public StrategyInterface
{
public:
	~CombinedStrategy() override = default;
	char run() override;
	void addHist(char s2, char s3) override;
	const std::string &getName() const override;

private:
	const std::string name = "combined";
	char countD;
	char countC;
};