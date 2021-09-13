#include "Agent.h"

double Agent::AGENT_HUNGER_SCORE = 0.0314152;
double Agent::AGENT_HUNGER_SCORE_STEEPNESS = 0.017735;


double scoreAgentHunger(double hunger)
{
	using namespace std;

	double t = GameWorldTime::get()->getTimeSpeed();
	double r = Agent::AGENT_HUNGER_SCORE + Agent::AGENT_HUNGER_SCORE * Agent::AGENT_HUNGER_SCORE_STEEPNESS;

	r *= t;

	cout << "HungerPrev: " << hunger << "HungerNew: " << r << endl;

	return r;
}

double logScoringFunction(double v)
{
	return std::log(v) * GameWorldTime::get()->getTimeSpeed();
}

void Agent::update(double dt)
{
	using namespace std;

	// Update the needs of the agent.
	scoreNeeds();


	if (actionStack.empty())
	{
		// Agent has nothing to do.

		// Get the need to battle.
		std::string need = getNeedToBattle();


		// Search for object that fulfills the need.
		GameObject* go = getObjectToFulfillNeedWith(need);


		if (go)
		{
			// There exists a gameobject 

			// Push actions on stack:
			// - Action associated with the need to fulfill
			// - Action to let agent move to fulfilling object
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
			else if (actionName.compare("ActionDoNothing") == 0)
			{
				action = ActionDatabase::get()->constructAction<ActionDoNothing>(actionName, this, go);
				actionStack.push(action);
			}


			// Get the position of destination object
			int x, y;
			TransformCmp* transf = go->getComponent<TransformCmp>("Transform");
			x = transf->xpos;
			y = transf->ypos;

			// Create special action which lets the agent just move to destination
			action = ActionDatabase::get()->constructAction<ActionMoveToDestination>(actionName, this, go, x, y);
			actionStack.push(action);
		}
		else
		{
			// There exists no gameobject or the needs does not require one

			// Currently, just create an idle action..
			// Where the agent goes to some location and the just waits

			ActionInstance* action = ActionDatabase::get()->constructAction<ActionDoNothing>("ActionDoNothing", this, nullptr);
			actionStack.push(action);

			TransformCmp* cmp = getComponent<TransformCmp>("Transform");
		
			int x = cmp->xpos;
			int y = cmp->ypos;

			action = ActionDatabase::get()->constructAction<ActionMoveToDestination>("ActionMoveToDestination", this, nullptr, x, y);
			actionStack.push(action);
		}



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


bool Agent::isTargetDestinationValid(int x, int y)
{
	if (x > NavMesh::get()->getWidth() ||
		y > NavMesh::get()->getHeight() ||
		x < 0 ||
		y < 0)
	{
		return false;
	}


	std::vector<std::vector<int>> v = NavMesh::get()->getNavGraph();

	for (int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j < v[i].size(); j++)
		{
			if (x == i || y == j)
			{
				if (v[i][j] != 1)
				{
					return false;
				}
			}
		}
	}

	return true;
}


void Agent::scoreNeeds()
{
	using namespace std;

	AgentNeedsCmp* needs = getComponent<AgentNeedsCmp>("AgentNeeds");
	double timedt = GameWorldTime::get()->getTimeSpeed();

	// Score sleep
	double sleep = score(scoreAgentHunger, needs->getSleep());
	needs->incrementSleep(sleep);


	// Score hunger
	double hunger = score(scoreAgentHunger, needs->getHunger());
	needs->incrementHunger(hunger);


	// Score thirst
	double thirst = score(scoreAgentHunger, needs->getThirst());
	needs->incrementThirst(thirst);


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
		if (n.second > 50.0 && n.second > max)
		{
			need = n.first;
			max = n.second;
		}
	}

	return need;
}


GameObject* Agent::getObjectToFulfillNeedWith(const std::string& need)
{
	GameObject* go = nullptr;
	double satisfaction_level = 0.0;

	// Check for a valid need
	if (need.compare("none") == 0) return nullptr;


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