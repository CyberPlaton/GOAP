#pragma once

#include "Action.h"

class TestingAction : public Action
{
public:
	TestingAction(const std::string& filepath)
	{
		init(filepath);
	}


	// Perform the action.
	bool perform() override
	{
		return false;
	}

	// What happens after the action is successfully achieved.
	bool postPerform() override
	{
		return false;
	}


};
