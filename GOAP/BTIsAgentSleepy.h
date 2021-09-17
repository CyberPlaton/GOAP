#pragma once

#include "ColorConsole.h"

#include "GameObject.h"
#include "BTCondition.h"

#include "AgentNeeds.h"


class BTIsAgentSleepy : public BTCondition
{
public:
	BTIsAgentSleepy(const std::string& name, GameObject* pawn) : BTCondition(name), pawn(pawn)
	{

	}


	BTNodeResult checkCondition() override
	{
		using namespace std;

		AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");

		if (needs->getNeed("Sleep") > 75.0f)
		{
			cout << color(colors::GREEN);
			cout << "[BTIsAgentSleepy] Agent \"" << pawn->getName() << "\" is sleepy" << white << endl;

			return BTNodeResult::SUCCESS;
		}
		else
		{
			cout << color(colors::RED);
			cout << "[BTIsAgentSleepy] Agent \"" << pawn->getName() << "\" is not sleepy" << white << endl;

			return BTNodeResult::FAILURE;
		}
	}


private:

	GameObject* pawn = nullptr;
};