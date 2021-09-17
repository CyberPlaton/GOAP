#pragma once

#include "ColorConsole.h"

#include "GameObject.h"
#include "BTCondition.h"

#include "AgentNeeds.h"


class BTIsAgentThirsty : public BTCondition
{
public:
	BTIsAgentThirsty(const std::string& name, GameObject* pawn) : BTCondition(name), pawn(pawn)
	{

	}


	BTNodeResult checkCondition() override
	{
		AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");

		if (needs->getNeed("Thirst") > 50.0f)
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