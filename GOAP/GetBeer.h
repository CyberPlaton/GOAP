#pragma once

#include "Action.h"

class AGetBeer : public Action
{
public:

	AGetBeer()
	{
	}


	bool postInit(Agent* this_agent, GameObject* target);


	// Perform the action.
	bool perform() override;

	// What happens after the action is successfully achieved.
	bool postPerform()  override;
};