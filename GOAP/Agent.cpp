#include "Agent.h"

void Agent::update(double dt)
{
	// Update the needs of the agent.
	scoreNeeds();


	if (actionStack.empty())
	{
		// Agent has nothing to do.

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
	double timedt = GameWorldTime::get()->getTimeSpeed();

	// Score sleep
	needs->incrementSleep(0.13 * timedt);

	// Score hunger
	needs->incrementHunger(0.03 * timedt);

	// Score thirst
	needs->incrementThirst(0.07 * timedt);


	// Store scores
	needsScoreMap["Sleep"] = needs->getSleep();
	needsScoreMap["Hunger"] = needs->getHunger();
	needsScoreMap["Thirst"] = needs->getThirst();
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
	double satisfaction_level = 0.0;

	// First search in invenory and owned objects
	// and choose the object which satisfies te most
	for (auto& o : agentInventory->getItems())
	{
		if (o->hasComponent("SmartObject"))
		{
			SmartObject* smo = o->getComponent<SmartObject>("SmartObject");

			double d = smo->getFulfillmentAmount(need);
			if (d > 0.0)
			{
				if (d > satisfaction_level)
				{
					go = o;
					satisfaction_level = d;
				}
			}
		}
	}

	if (go) return go;



	for (auto& o : agentOwnedObjects)
	{
		if (o->hasComponent("SmartObject"))
		{
			SmartObject* smo = o->getComponent<SmartObject>("SmartObject");

			double d = smo->getFulfillmentAmount(need);
			if (d > 0.0)
			{
				if (d > satisfaction_level)
				{
					go = o;
					satisfaction_level = d;
				}
			}
		}
	}

	if (go) return go;


	// Then search in world.
	std::vector< GameObject* >& storage = GameObjectStorage::get()->getStorage();
	for (auto& o : storage)
	{
		if (o->hasComponent("SmartObject"))
		{
			SmartObject* smo = o->getComponent<SmartObject>("SmartObject");

			double d = smo->getFulfillmentAmount(need);
			if (d > 0.0)
			{
				if (d > satisfaction_level)
				{
					go = o;
					satisfaction_level = d;
				}
			}
		}
	}

	if (go) return go;


	// Nothing found to fulfill need with
	return nullptr;
}