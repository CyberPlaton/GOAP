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
	void setHunger(double n)
	{ 
		if (n < 0.0)
		{
			hunger = 0.0;
		}
		else if (n > 100.0)
		{
			hunger = 100.0;
		}
		else
		{
			hunger = n;
		}
	};

	void setThirst(double n)
	{
		if (n < 0.0)
		{
			thirst = 0.0;
		}
		else if (n > 100.0)
		{
			thirst = 100.0;
		}
		else
		{
			thirst = n;
		}
	};

	void setSleep(double n)
	{
		if (n < 0.0)
		{
			sleep = 0.0;
		}
		else if (n > 100.0)
		{
			sleep = 100.0;
		}
		else
		{
			sleep = n;
		}
	};


	double getHunger() { return hunger; };
	double getThirst() { return thirst; };
	double getSleep() { return sleep; };


private:
	ComponentType type;

	double hunger = 0.0;
	double thirst = 0.0;
	double sleep = 0.0;

};