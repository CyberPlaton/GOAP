#pragma once

#include "ColorConsole.h"

#include "GameObject.h"
#include "BTAction.h"

#include "BTBlackboard.h"
#include "OwnershipCmp.h"

/*
* BTFindAgentBed tries to find a bed that belongs to the agent,
* otherwise it returns failure.
*/
class BTFindAgentBed : public BTAction
{
public:
	BTFindAgentBed(const std::string& name, GameObject* pawn, BTBlackboard* agentBlackboard) :
		BTAction(name), pawn(pawn), blackboard(agentBlackboard)
	{
	}

	BTNodeResult command() override
	{
		using namespace std;

		GameObject* bed = nullptr;
		OwnershipCmp* owned = pawn->getComponent<OwnershipCmp>("Ownership");

		if (!owned->doesOwnObjects())
		{
			// Default out to failure.
		}
		else if (blackboard->getData<GameObject*>("Bed") != nullptr)
		{
			cout << color(colors::GREEN);
			cout << "[BTFindAgentBed] Agent \"" << pawn->getName() << "\" already has access to a bed" << white << endl;

			return BTNodeResult::SUCCESS;
		}
		else if (bed = owned->getOwnedObject("Bed"); bed != nullptr)
		{
			cout << color(colors::GREEN);
			cout << "[BTFindAgentBed] Found bed for \"" << pawn->getName() << "\"" << white << endl;


			// We found a bed belonging to the agent, store it in the blackboard and return success.
			blackboard->set<GameObject*>("Bed", bed, "GameObject");

			return BTNodeResult::SUCCESS;
		}



		cout << color(colors::RED);
		cout << "[BTFindAgentBed] Bed not found for \"" << pawn->getName() << "\"" << white << endl;

		return BTNodeResult::FAILURE;
	}


private:
	GameObject* pawn = nullptr;
	BTBlackboard* blackboard = nullptr;
};