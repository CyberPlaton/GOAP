#pragma once

#include "ColorConsole.h"

#include "GameObject.h"
#include "Components.h"
#include "BTAction.h"

#include "AgentNeeds.h"

class BTAgentSleep : public BTAction
{
public:
	BTAgentSleep(const std::string& name, GameObject* pawn) :
		BTAction(name), pawn(pawn)
	{
	}

	BTNodeResult command() override
	{
		using namespace std;

		// The agent will execute this function until the sleep is down to zero.

		// Check need
		AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");
		float sleep = needs->getNeed("Sleep");

		if (sleep <= 1.0f)
		{
			cout << color(colors::DARKGREEN);
			cout << "Sleep need of \"" << pawn->getName() << "\" is fulfilled" << white << endl;

			return BTNodeResult::SUCCESS;
		}
		else
		{
			cout << color(colors::DARKGREEN);
			cout << "Sleep need of \"" << pawn->getName() << "\" is reset to 1.0" << white << endl;

			needs->setNeed("Sleep", 1.0f);
			return BTNodeResult::SUCCESS;
		}
	}


private:
	GameObject* pawn = nullptr;
};