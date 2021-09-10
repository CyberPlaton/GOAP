#pragma once

#include "ComponentSystem.h"

class AgentStatsCmp : public Component
{
public:
	AgentStatsCmp(const ComponentID& name)
	{
		this->name = name;
		type = "AgentStats";
		init(type);
	}

	ComponentType getType() override { return this->type; }


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
	ComponentType type;

	double strength = 0.0;
	double intellect = 0.0;
	double willpower = 0.0;
	double agility = 0.0;
	double speed = 0.0;
	double endurance = 0.0;
	double personality = 0.0;
	double luck = 0.0;
	double health = 0.0;
	double magicka = 0.0;
	double fatigue = 0.0;
};