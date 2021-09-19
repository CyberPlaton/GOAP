#pragma once

#include "ColorConsole.h"

#include "GameObject.h"
#include "BTAction.h"

#include "BTBlackboard.h"
#include "OwnershipCmp.h"
#include "Inventory.h"

/*
* BTFindAgentObject searches for given gameobject and stores it in blackboard with given name.
*/
class BTFindAgentObject : public BTAction
{
public:
	enum SearchPolicy
	{
		SP_ALL,
		SP_OWNERSHIP,
		SP_INVENTORY,
		SP_OWNERSHIP_INVENTORY,
	};

public:
	BTFindAgentObject(const std::string& name, GameObject* pawn, const std::string& searchedObjectName, SearchPolicy searchPolicy, BTBlackboard* agentBlackboard,
		const std::string& objectStoreName) :
		BTAction(name), pawn(pawn), blackboard(agentBlackboard), searchPolicy(searchPolicy), objectStoreName(objectStoreName), searchedObjectName(searchedObjectName)

	{
	}

	BTNodeResult command() override
	{
		using namespace std;

		GameObject* searchedFor = nullptr;
		bool extended_search = false;

		// As we search in the ownership component,
		// we can use "searchedObjectName" to find any object matching the name: getOwnedObject( searchedObjectName )
		// As we search on inventory component,
		// we have to remember to match the object name in the inventory, the definition file for constructing it and the "searchedObjectName",
		// later we should consider creating a mapping for object names and filepaths...


		switch (searchPolicy)
		{
		case SearchPolicy::SP_ALL:
			extended_search = true;
		case SearchPolicy::SP_OWNERSHIP_INVENTORY:
			extended_search = true;
		case SearchPolicy::SP_OWNERSHIP:

			searchedFor = pawn->getComponent<OwnershipCmp>("Ownership")->getOwnedObject(searchedObjectName);
			if (searchedFor)
			{

				break;
			}

			if (!extended_search) break;

		case SearchPolicy::SP_INVENTORY:

			searchedFor = pawn->getComponent<InventoryCmp>("Inventory")->getItem(searchedObjectName);
			if(searchedFor)
			{

				break;
			}

			if (!extended_search) break;

		default:
			break;
		}


		// Store the found object, if nothing was found nullptr is stored.
		blackboard->set<GameObject*>(objectStoreName, searchedFor, "GameObject");


		if (!searchedFor)
		{
			cout << color(colors::RED);
			cout << "[BTFindAgentObject] Not Found \""<< searchedObjectName <<"\" for \"" << pawn->getName() << "\"" << white << endl;
			return BTNodeResult::FAILURE;

		}
		else
		{
			cout << color(colors::GREEN);
			cout << "[BTFindAgentObject] Found \""<< searchedObjectName << "\" for \"" << pawn->getName() << "\"" << white << endl;
			return BTNodeResult::SUCCESS;

		}
	}


private:
	GameObject* pawn = nullptr;
	BTBlackboard* blackboard = nullptr;
	SearchPolicy searchPolicy;
	std::string objectStoreName;
	std::string searchedObjectName;
};