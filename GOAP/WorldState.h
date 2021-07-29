#pragma once

#include <map>
#include <string>

class WorldState
{
public:
	WorldState(const std::string& key, int value):key(key), value(value) {}

	std::string key;
	int value;
};



class WorldStates
{
public:


	bool hasState(const std::string& key)
	{
		return states.contains(key);
	}


	/*
	* Add Value to a state if it exists, else add new one.
	*/
	void modifyState(const std::string& key, int value)
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
	void setState(const std::string& key, int value)
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


	void addState(const std::string& key, int value)
	{
		states.emplace(std::make_pair(key, value));
	}

	std::map<std::string, int> getStates()
	{
		return states;
	}



private:

	std::map<std::string, int> states;
};