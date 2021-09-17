#pragma once

#include "ColorConsole.h"

#include "GameObject.h"
#include "Components.h"
#include "BTAction.h"

#include "BTBlackboard.h"

#include "GameWorldTime.h"


class BTGoToAgentBed : public BTAction
{
public:
	BTGoToAgentBed(const std::string& name, GameObject* pawn, BTBlackboard* agentBlackboard) :
		BTAction(name), pawn(pawn), blackboard(agentBlackboard)
	{
	}

	BTNodeResult command() override
	{
		using namespace std;

		cout << color(colors::CYAN);
		cout << "[BTGoToAgentBed] Moving \"" << pawn->getName() << "\" to destination..." << white << endl;

		if (!destinationSet)
		{
			GameObject* bed = nullptr;

			// Get bed from blackboard.
			bed = blackboard->getData<GameObject*>("Bed");

			// Get position of the bed and set it as the destination for the agents navigator.
			TransformCmp* transform = bed->getComponent<TransformCmp>("Transform");

			NavigatorCmp* navi = pawn->getComponent<NavigatorCmp>("Navigator");
			navi->setDestination(transform->xpos, transform->ypos);

			destinationSet = true;

			cout << color(colors::GREEN);
			cout << "[BTGoToAgentBed] Destination for \"" << pawn->getName() << "\" set to ";
			cout << "{"<< transform->xpos << ", "<< transform->ypos << "}" << white << endl;

			return BTNodeResult::RUNNING;
		}
		else
		{

			// If we do not have reached our destination we move towards it.
			NavigatorCmp* navi = pawn->getComponent<NavigatorCmp>("Navigator");


			if (navi->isDestinationReached())
			{
				destinationSet = false;
				return BTNodeResult::SUCCESS;
			}
			else
			{
				return BTNodeResult::RUNNING;
			}
		}


		return BTNodeResult::FAILURE;
	}


private:
	GameObject* pawn = nullptr;
	BTBlackboard* blackboard = nullptr;

	bool destinationSet = false;
};