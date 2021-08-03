#pragma once

#include "Action.h"


class AEat : public Action
{
public:

	AEat()
	{
	}

	/*
	* Implemented in Agent.cpp
	*/
	bool postInit(Agent* this_agent, GameObject* target);


	// Perform the action.
	bool perform() override;

	// What happens after the action is successfully achieved.
	bool postPerform()  override;
};
