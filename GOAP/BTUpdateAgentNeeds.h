#pragma once

#include "ColorConsole.h"

#include "GameObject.h"
#include "BTAction.h"

#include "GameWorldTimer.h"

#include "AgentNeeds.h"


/*
* Update the needs of the agent.
*/
class BTUpdateAgentNeeds : public BTAction
{
public:
	BTUpdateAgentNeeds(const std::string& name, GameObject* pawn) : BTAction(name), pawn(pawn)
	{
		timer = new GameworldTimer();
		timer->startTimer(); // Initial start.
	}

	BTNodeResult command() override
	{
		using namespace std;

		if (timer->getElapsedGamehours() > 0.30)
		{
			cout << color(colors::GREEN);
			cout << "[BTUpdateAgentNeeds] Updating needs of \""<< pawn->getName() << "\"" << white << endl;

			AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");

			needs->incrementNeed("Hunger", 2.5f);
			needs->incrementNeed("Sleep", 8.4f);
			needs->incrementNeed("Thirst", 2.9f);


			// In the end restart the timer.
			timer->startTimer();
			return BTNodeResult::SUCCESS;
		}
		else
		{
			cout << color(colors::RED);
			cout << "[BTUpdateAgentNeeds] Not updating \"" << pawn->getName() << "\"s needs" << white << endl;
			return BTNodeResult::RUNNING;
		}
	}


private:
	GameObject* pawn = nullptr;

	GameworldTimer* timer = nullptr;
};