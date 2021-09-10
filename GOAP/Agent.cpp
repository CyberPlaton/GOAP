#include "Agent.h"

void Agent::update(double dt)
{
	if (actionStack.empty())
	{
		// Agent has nothing to do.


		// Score needs.
		scoreNeeds();


		// Get the need to battle.
		std::string need = getNeedToBattle();


		// Search for object that fulfills the need.
		GameObject* go = getObjectToFulfillNeedWith(need);


		// Push actions on stack:
		// Action associated with the need to fulfill
		// Action to let agent move to fulfilling object
		SmartObject* smo = go->getComponent<SmartObject>("SmartObject");
		std::string actionName = smo->getAssociatedAction(need);


		ActionInstance* action = nullptr;
		if (actionName.compare("ActionEat") == 0)
		{
			action = ActionDatabase::get()->constructAction<ActionEat>(actionName, this, go);
			actionStack.push(action);
		}
		else if (actionName.compare("ActionSleep") == 0)
		{
			action = ActionDatabase::get()->constructAction<ActionSleep>(actionName, this, go);
			actionStack.push(action);
		}
		else if (actionName.compare("ActionDrink") == 0)
		{
			action = ActionDatabase::get()->constructAction<ActionDrink>(actionName, this, go);
			actionStack.push(action);
		}


		// Get the position of destination object
		int x, y;
		TransformCmp* transf = go->getComponent<TransformCmp>("Transform");
		x = transf->xpos;
		y = transf->ypos;

		action = ActionDatabase::get()->constructAction<ActionMoveToDestination>(actionName, this, go, x, y);
		actionStack.push(action);

		// return
	}
	else
	{
		// Agent has a goal and set of actions.


		// Get latest action from stack
		ActionInstance* action = actionStack.top();

		// Perform it
		bool result = false;
		if (action->perform(dt))
		{
			result = true;
		}


		// Postperform and pop it, if it returned true
		if (result)
		{
			action->postPerform(dt);
			actionStack.pop();
		}
	}
}


void Agent::scoreNeeds()
{
	using namespace std;

	AgentNeedsCmp* needs = getComponent<AgentNeedsCmp>("AgentNeeds");

	double r = static_cast<double>(rand() % 5);


	// Score sleep
	needs->setSleep(needs->getSleep() + r * 1.3);

	// Score hunger
	needs->setHunger(needs->getHunger() + r * 0.3);

	// Score thirst
	needs->setThirst(needs->getThirst() + r * 0.7);


	// Store scores
	needsScoreMap["Sleep"] = needs->getSleep();
	needsScoreMap["Hunger"] = needs->getHunger();
	needsScoreMap["Thirst"] = needs->getThirst();


	cout << "Needs of \""<< getName() << "\": " << endl;
	cout << "\"Sleep\": " << needs->getSleep() << endl;
	cout << "\"Hunger\": " << needs->getHunger() << endl;
	cout << "\"Thirst\": " << needs->getThirst() << endl;
}


std::string Agent::getNeedToBattle()
{
	// For now just return highest need.
	double max = 0.0;
	std::string need = "none";
	for (auto& n : needsScoreMap)
	{
		if (n.second > max)
		{
			need = n.first;
			max = n.second;
		}
	}


	if (need.compare("none") == 0)
	{
		return "Sleep";
	}

	return need;
}


GameObject* Agent::getObjectToFulfillNeedWith(const std::string& need)
{
	GameObject* go = nullptr;

	// First search in invenory and owned objects.
	for (auto& o : agentInventory->getItems())
	{
		if (o->hasComponent("SmartObject"))
		{
			SmartObject* smo = o->getComponent<SmartObject>("SmartObject");

			if (smo->getFulfillmentAmount(need) > 0.0)
			{
				return o;
			}
		}
	}

	for (auto& o : agentOwnedObjects)
	{
		if (o->hasComponent("SmartObject"))
		{
			SmartObject* smo = o->getComponent<SmartObject>("SmartObject");

			if (smo->getFulfillmentAmount(need) > 0.0)
			{
				return o;
			}
		}
	}


	// Then search in world.
	std::vector< GameObject* >& storage = GameObjectStorage::get()->getStorage();
	for (auto& o : storage)
	{
		if (o->hasComponent("SmartObject"))
		{
			SmartObject* smo = o->getComponent<SmartObject>("SmartObject");

			if (smo->getFulfillmentAmount(need) > 0.0)
			{
				return o;
			}
		}
	}


	// Nothing found to fulfill need with
	return nullptr;
}