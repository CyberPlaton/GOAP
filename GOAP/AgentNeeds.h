#pragma once

#include "ComponentSystem.h"

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

	/*
	* Basic needs.
	*/
	void setHunger(double n) { hunger = n; };
	void setThirst(double n) { thirst = n; };
	void setSleep(double n) { sleep = n; };

	double getHunger() { return hunger; };
	double getThirst() { return thirst; };
	double getSleep() { return sleep; };


private:
	ComponentType type;

	double hunger = 0.0;
	double thirst = 0.0;
	double sleep = 0.0;

};