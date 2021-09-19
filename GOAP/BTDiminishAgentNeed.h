#pragma once

#include "ColorConsole.h"

#include "GameObject.h"
#include "Components.h"
#include "BTAction.h"

#include "AgentNeeds.h"


class BTDiminishAgentNeed : public BTAction
{
public:
	BTDiminishAgentNeed(const std::string& name, GameObject* pawn, const std::string& needName, float needAmount) :
		BTAction(name), pawn(pawn), needName(needName), amount(needAmount)
	{
	}

	BTNodeResult command() override
	{
		using namespace std;

		// The agent will execute this function until the sleep is down to zero.

		// Check need
		AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");
		float need = needs->getNeed(needName);

		if (need <= 10.0f)
		{
			cout << color(colors::DARKGREEN);
			cout << "Sleep need of \"" << pawn->getName() << "\" is fulfilled" << white << endl;

			return BTNodeResult::SUCCESS;
		}
		else
		{
			needs->incrementNeed(needName, -amount);
		
			cout << color(colors::DARKGREEN);
			cout << "Sleep need of \"" << pawn->getName() << "\" is diminished by " << amount;
			cout << ", current: \""<< needName << "\"=" << needs->getNeed(needName);
			
			return BTNodeResult::RUNNING;
		}
	}


private:
	GameObject* pawn = nullptr;
	std::string needName;
	float amount = 0.0f;
};