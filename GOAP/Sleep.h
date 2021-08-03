#pragma once

#include "Action.h"

class ASleep : public Action
{
public:

	ASleep()
	{
	}


	bool postInit(Agent* this_agent, GameObject* target);


	// Perform the action.
	bool perform() override;

	// What happens after the action is successfully achieved.
	bool postPerform()  override;
};