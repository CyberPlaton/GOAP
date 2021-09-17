#pragma once

#include "ColorConsole.h"

#include "GameObject.h"
#include "BTCondition.h"

#include "AgentNeeds.h"


class BTIsAgentHungry : public BTCondition
{
public:
	BTIsAgentHungry(const std::string& name, GameObject* pawn) : BTCondition(name), pawn(pawn)
	{

	}


	BTNodeResult checkCondition() override
	{
		AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");

		if (needs->getNeed("Hunger") > 50.0f)
		{
			return BTNodeResult::SUCCESS;
		}
		else
		{
			return BTNodeResult::FAILURE;
		}
	}


private:

	GameObject* pawn = nullptr;
};