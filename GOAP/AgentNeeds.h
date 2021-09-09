#pragma once

#include "ComponentSystem.h"

class AgentNeedsCmp : public Component
{
public:
	AgentNeedsCmp(const std::string& name)
	{
		this->name = name;
		type = "AgentNeeds";
		init(type);
	}

	std::string getType() override { return this->type; }

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
	std::string type;

	double hunger;
	double thirst;
	double sleep;

};