#pragma once

#include "ComponentSystem.h"

using AgentNeed = std::string;

class AgentNeedsCmp : public Component
{
public:
	AgentNeedsCmp(const ComponentID& name)
	{
		this->name = name;
		type = "AgentNeeds";
		init(type);
	}

	ComponentType getType() override { return this->type; }

	// Create a new need entry with given value. Or set directly the value of an 
	// existing entry.
	void setNeed(const AgentNeed& need, float n)
	{
		needs[need] = n;
	}

	// Increment the value if it exists, else do nothing.
	void incrementNeed(const AgentNeed& need, float n)
	{
		float v = getNeed(need);
		if (v != (float)INT_MAX)
		{
			setNeed(need, v + 1);
		}
	}

	// Get value of a need, if it does not exist returns infinity.
	float getNeed(const AgentNeed& need)
	{
		auto it = needs.find(need);
		if (it != needs.end())
		{
			return needs[need];
		}

		return (float)INT_MAX;
	}


private:
	ComponentType type;

	std::map<AgentNeed, float> needs;
};