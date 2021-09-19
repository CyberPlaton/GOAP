#pragma once

#include "ColorConsole.h"

#include "GameObject.h"
#include "Components.h"
#include "BTAction.h"

#include "BTBlackboard.h"

#include "GameWorldTime.h"


class BTGoToAgentObject : public BTAction
{
public:
	BTGoToAgentObject(const std::string& name, GameObject* pawn, BTBlackboard* agentBlackboard, const std::string& objectName) :
		BTAction(name), pawn(pawn), blackboard(agentBlackboard), destObjectName(objectName)
	{
	}

	BTNodeResult command() override
	{
		using namespace std;


		if (!destinationSet)
		{
			GameObject* dest = nullptr;

			// Get bed from blackboard.
			dest = blackboard->getData<GameObject*>(destObjectName);
			if (!dest) return BTNodeResult::FAILURE;


			// Get position of the bed and set it as the destination for the agents navigator.
			TransformCmp* transform = dest->getComponent<TransformCmp>("Transform");


			NavigatorCmp* navi = pawn->getComponent<NavigatorCmp>("Navigator");
			navi->setDestination(transform->xpos, transform->ypos);


			destinationSet = true;


			cout << color(colors::GREEN);
			cout << "[BTGoToAgentBed] Destination for \"" << pawn->getName() << "\" set to ";
			cout << "{" << transform->xpos << ", " << transform->ypos << "} ";
			cout << "to Gameobject \""<< dest->getName() << "\"" << white << endl;

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
	}


private:
	GameObject* pawn = nullptr;
	BTBlackboard* blackboard = nullptr;

	std::string destObjectName;

	bool destinationSet = false;
};