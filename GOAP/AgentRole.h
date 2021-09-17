#pragma once

#include "BTInterface.h"
#include "ComponentSystem.h"

/*
* The agent role is basically its tree,
* that executes predefined behavior.
* 
* However, it is not responsible for executing the tree logic.
*/
class AgentRoleCmp : public Component
{
public:
	AgentRoleCmp(const ComponentID& name, const std::string& role_name)
		: role(role_name)
	{
		this->name = name;
		type = "AgentRole";
		init(type);
	}

	ComponentType getType() override { return this->type; }


private:
	std::string role;
	ComponentType type;



private:


};