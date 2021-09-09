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
	AgentRoleCmp(const std::string& name, const std::string& role_name)
		: role(role_name)
	{
		this->name = name;
		type = "AgentRole";
		init(type);
	}


	bool init(const std::string& tree_path)
	{
		return 	_loadTree(tree_path);
	}

	bool init(BehaviorTree* tree)
	{
		this->tree = tree;
		return this->tree != nullptr;
	}

	std::string getType() override { return this->type; }


private:
	std::string role;
	std::string type;

	BehaviorTree* tree = nullptr;


private:

	bool _loadTree(const std::string& path)
	{

		return false;
	}
};