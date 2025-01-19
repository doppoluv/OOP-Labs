#pragma once

#include "strategies.h"
#include <map>
#include <memory>

class CreatorInterface
{
public:
	virtual ~CreatorInterface() {};
	virtual std::unique_ptr<StrategyInterface> create() = 0;
};

template <class C>
class Creator : public CreatorInterface
{
public:
	std::unique_ptr<StrategyInterface> create() override
	{
		return std::make_unique<C>();
	}
	~Creator() override = default;
};

class Factory
{
public:
	std::unique_ptr<StrategyInterface> create(const std::string &id)
	{
		auto it = m_factory.find(id);
		if (it == m_factory.end())
		{
			return nullptr;
		}
		return it->second->create();
	}
	void insert(std::string id, std::unique_ptr<CreatorInterface> creator)
	{
		m_factory.insert({std::move(id), std::move(creator)});
	}

private:
	std::map<std::string, std::unique_ptr<CreatorInterface>> m_factory;
};