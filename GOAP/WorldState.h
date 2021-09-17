#pragma once

#include <map>
#include <string>

#include "ComponentSystem.h"


class WorldStateCmp : public Component
{
public:
	WorldStateCmp(const ComponentID& name)
	{
		this->name = name;
		type = "WorldState";
		init(type);
	}

	~WorldStateCmp()
	{
		states.clear();
	}


	ComponentType getType() override { return this->type; }


	bool hasState(const std::string& key)
	{
		return states.contains(key);
	}


	/*
	* Add given Value to a state if it exists, else add new one.
	*/
	void modifyState(const std::string& key, double value)
	{
		if (hasState(key))
		{
			states[key] += value;

			if (states[key] <= 0)
			{
				removeState(key);
			}
		}
		else
		{
			addState(key, value);
		}
	}


	/*
	* If the state exists, remove it completely from the states.
	*/
	void removeState(const std::string& key)
	{
		auto it = states.find(key);

		if (it != states.end())
		{
			states.erase(it);
		}
	}


	/*
	* Set value of state directly to a value, or insert a new state.
	*/
	void setStateValue(const std::string& key, double value)
	{
		if (hasState(key))
		{
			states[key] = value;
		}
		else
		{
			addState(key, value);
		}
	}


	void addState(const std::string& key, double value)
	{
		states.emplace(std::make_pair(key, value));
	}


	std::map<std::string, double>& getStates()
	{
		return states;
	}



private:
	ComponentType type;

	std::map<std::string, double> states;
};