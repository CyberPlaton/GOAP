#include "Agent.h"

double Agent::AGENT_HUNGER_SCORE = 0.0064152;
double Agent::AGENT_HUNGER_SCORE_STEEPNESS = 0.00012735;
double Agent::AGENT_SLEEP_SCORE = 0.00121152;
double Agent::AGENT_SLEEP_SCORE_STEEPNESS = 0.61135;



double scoreAgentHunger(GameObject* pawn, double hunger)
{
	using namespace std;

	double t = GameWorldTime::get()->getTimeSpeed();
	double r = 0.0;
	hunger = hunger + 0.001;

	r = t * (hunger * Agent::AGENT_HUNGER_SCORE_STEEPNESS + Agent::AGENT_HUNGER_SCORE);

	cout << color(colors::DARKRED);
	cout << "Increase Hunger by " << r;
	cout << ", where Hunger =" << hunger << white << endl;

	return r;
}


double scoreAgentSleep(GameObject* pawn, double sleep)
{
	using namespace std;

	double t = GameWorldTime::get()->getTimeSpeed();
	double r = 0.0;
	sleep = sleep + 0.001;

	r = t * ( 1 / sleep * Agent::AGENT_SLEEP_SCORE_STEEPNESS + Agent::AGENT_SLEEP_SCORE);

	// Modify the sleep score if agent has slept lastly.
	if (static_cast<Agent*>(pawn)->agentBeliefs->getStates()["HasSlept"] == 1.0)
	{
		r -= Agent::AGENT_SLEEP_SCORE * 2;
	
		cout << color(colors::GREEN);
		cout << "Decreasing SleepScore by " << Agent::AGENT_SLEEP_SCORE * 2;
		cout << ", because Agent has slept lastly" << white << endl;
	}

	cout << color(colors::DARKCYAN);
	cout << "Increase Sleep by " << r;
	cout << ", where Sleep =" << sleep << white << endl;

	return r;
}



void Agent::update(double dt)
{
	using namespace std;

	// Update the needs of the agent.
	scoreNeeds();


	// Check for beliefs which need to be altered.
	// Slept lastly benefit
	if (timer->getElapsedGamehours() > 5.0)
	{
		agentBeliefs->setState("HasSlept", 0.0);

		cout << color(colors::DARKGREEN);
		cout << "Reset \"HasSlept\" benefit" << white << endl;
	}


	if (actionStack.empty())
	{
		// Agent has nothing to do.

		// Get the need to battle.
		std::string need = getMostValuableGoal();

		// Search for object that fulfills the need.
		GameObject* go = getObjectToFulfillNeedWith(need);


		if (go)
		{
			// There exists a gameobject that satisfies the need

			// Push actions on stack:
			// - Action associated with the need to fulfill
			// - Action to let agent move to fulfilling object

			// Create appropriate action for smartobject and need
			Action* action = getActionOfSmartObjectForNeed(go, need);
			
			// Push the smartobject action on the stack to execute after agent arrived
			actionStack.push(action);


			// Get the position of destination object
			int x, y;
			TransformCmp* transf = go->getComponent<TransformCmp>("Transform");
			x = transf->xpos;
			y = transf->ypos;

			// Create special action which lets the agent just move to destination
			Action* move = ActionDatabase::get()->constructAction<ActionMoveToDestination>("ActionMoveToDestination", this, go, x, y);
			actionStack.push(move);
		}
		else
		{
			// There exists no gameobject or the needs does not require one



			// Create an "IDLE" action, where agent wanders around and does nothing.
			Action* action = ActionDatabase::get()->constructAction<ActionDoNothing>("ActionDoNothing", this, nullptr);
			actionStack.push(action);

			NavigatorCmp* nav = getComponent<NavigatorCmp>("Navigator");
			std::pair<int, int> dest = nav->getRandomDestinationAroundAgent(2);
			

			action = ActionDatabase::get()->constructAction<ActionMoveToDestination>("ActionMoveToDestination", this, nullptr, dest.first, dest.second);
			actionStack.push(action);
		}



		// return
	}
	else
	{
		// Agent has a goal and set of actions.


		// Get latest action from stack
		Action* action = actionStack.top();

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


Action* Agent::getActionOfSmartObjectForNeed(GameObject* go, const std::string& need)
{
	using namespace std;

	// Get the smartobject
	if (!go->hasComponent("SmartObject")) return nullptr;
	SmartObject* smo = go->getComponent<SmartObject>("SmartObject");


	// Get the action name for the need
	std::string actionName = smo->getAssociatedAction(need);
	if (actionName.compare("") == 0) return nullptr;


	// Create the action
	Action* action = nullptr;
	if (actionName.compare("ActionEat") == 0)
	{
		action = ActionDatabase::get()->constructAction<ActionEat>(actionName, this, go);
	}
	else if (actionName.compare("ActionSleep") == 0)
	{
		action = ActionDatabase::get()->constructAction<ActionSleep>(actionName, this, go);
	}
	else if (actionName.compare("ActionDrink") == 0)
	{
		action = ActionDatabase::get()->constructAction<ActionDrink>(actionName, this, go);
	}
	else if (actionName.compare("ActionDoNothing") == 0)
	{
		action = ActionDatabase::get()->constructAction<ActionDoNothing>(actionName, this, go);
	}
	else
	{
		cout << color(colors::RED);
		cout << "[AGENT::getActionOfSmartObjectForNeed] Undefined action \""<< actionName  <<"\"" << white << endl;
	}

	return action;
}


void Agent::scoreNeeds()
{
	using namespace std;

	AgentNeedsCmp* needs = getComponent<AgentNeedsCmp>("AgentNeeds");
	double timedt = GameWorldTime::get()->getTimeSpeed();

	// Score sleep
	double sleep = score(scoreAgentSleep, this, needs->getSleep());
	needs->incrementSleep(sleep);


	// Score hunger
	double hunger = score(scoreAgentHunger, this, needs->getHunger());
	needs->incrementHunger(hunger);


	// Score thirst
	double thirst = score(scoreAgentHunger, this, needs->getThirst());
	needs->incrementThirst(thirst);


	// Store scores
	needsScoreMap["Sleep"] = needs->getSleep();
	needsScoreMap["Hunger"] = needs->getHunger();
	needsScoreMap["Thirst"] = needs->getThirst();
}


std::string Agent::getMostValuableGoal()
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