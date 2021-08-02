#pragma once

#include "Action.h"

class ASleep : public Action
{
public:

	ASleep()
	{
	}


	bool postInit(GameObject* this_agent, GameObject* target)
	{
		this->target = target;
		this->this_agent = this_agent;

		this->target_name = target->name;
		this->this_agent_name = this_agent->name;

		return true;
	}


	// Perform the action.
	bool perform() override
	{
		return true;
	}

	// What happens after the action is successfully achieved.
	bool postPerform()  override
	{
		return true;
	}
};