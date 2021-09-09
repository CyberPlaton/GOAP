#pragma once

#include "ComponentSystem.h"

class AgentStatsCmp : public Component
{
public:
	AgentStatsCmp(const std::string& name)
	{
		this->name = name;
		type = "AgentStats";
		init(type);
	}

	std::string getType() override { return this->type; }


	void setStrength(double n) { strength = n; };
	void setIntellect(double n) { intellect = n; };
	void setWillpower(double n) { willpower = n; };
	void setAgility(double n) { agility = n; };
	void setSpeed(double n) { speed = n; };
	void setEndurance(double n) { endurance = n; };
	void setPersonality(double n) { personality = n; };
	void setLuck(double n) { luck = n; };

	void setHealth(double n) { health = n; };
	void setFatigue(double n) { fatigue = n; };
	void setMagicka(double n) { magicka = n; };


	double getStrength() { return strength; };
	double getIntellect() { return intellect; };
	double getWillpower() { return willpower; };
	double getAgility() { return agility; };
	double getSpeed() { return speed; };
	double getEndurance() { return endurance; };
	double getPersonality() { return personality; };
	double getLuck() { return luck; };
	
	double getHealth() { return health; };
	double getFatigue() { return fatigue; };
	double getMagicka() { return magicka; };

private:
	std::string type;

	double strength;
	double intellect;
	double willpower;
	double agility;
	double speed;
	double endurance;
	double personality;
	double luck;
	double health;
	double magicka;
	double fatigue;
};