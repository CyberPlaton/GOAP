#include "Agent.h"

std::map<std::string, std::string> Agent::role_definitions_map;


bool Agent::init(const std::string& filepath)
{
	using namespace std;
	using namespace nlohmann;

	ifstream fin(filepath);
	if (fin.is_open() == false) return false;

	json in;
	fin >> in;

	for (auto& entry : in.at("Actions"))
	{
		std::string name = entry[0].get<std::string>();
		std::string path = entry[1].get<std::string>();

		actionDefinitions.emplace(std::make_pair(name, path));
	}

	// Actually create the action and fill them in for the agent.
	for (auto& a : actionDefinitions)
	{
		if (a.first.compare("Sleep") == 0)
		{
			ASleep* action = new ASleep();
			if (!action->init(a.second)) // Get Definition.
			{
				continue; // Do not append the action, just ignore it.
			}

			_initAction(action);
		}
		else if (a.first.compare("Eat") == 0)
		{
			AEat* action = new AEat();
			if (!action->init(a.second)) // Get Definition.
			{
				continue; // Do not append the action, just ignore it.
			}

			_initAction(action);
		}
		else if (a.first.compare("Get Beer") == 0)
		{
			AGetBeer* action = new AGetBeer();
			if (!action->init(a.second)) // Get Definition.
			{
				continue; // Do not append the action, just ignore it.
			}

			_initAction(action);
		}

	}

	return true;
}


bool Agent::_initAction(Action* a)
{
	/*
	* As the definition for objects are defined agent agnostic, but each agent needs
	* to access the defined target which are available for him specifically,
	* we get the target from here.
	*/

	GameObject* target = nullptr;

	// Search for target in owned objects:
	for (auto& owned_thing : this->agentOwnedObjects)
	{
		if (owned_thing->getName().find(a->target_name) != std::string::npos)
		{
			target = owned_thing;
			break;
		}
	}


	// Too try search for target in the world environment.
	if (!target)
	{
		target = GameObjectStorage::get()->getGOByName(a->target_name);
	}


	if (!target) return false;


	if (!a->postInit(this, target)) // Specify the gameobjects.
	{
		return false;
	}

	a->awake();			// First life sign. Initialize needed structures.

	availableActions.push_back(a); // Store action as available.

	return true;
}








/*
* Definitions of the Action functions.
*/
bool AEat::postInit(Agent* this_agent, GameObject* target)
{
	this->target = target;
	this->this_agent = this_agent;

	this->target_name = target->name;
	this->this_agent_name = this_agent->name;

	return true;
}

bool ASleep::postInit(Agent* this_agent, GameObject* target)
{
	this->target = target;
	this->this_agent = this_agent;

	this->target_name = target->name;
	this->this_agent_name = this_agent->name;

	return true;
}

bool AGetBeer::postInit(Agent* this_agent, GameObject* target)
{
	this->target = target;
	this->this_agent = this_agent;

	this->target_name = target->name;
	this->this_agent_name = this_agent->name;

	return true;
}

bool AEat::perform()
{
	return true;
}

bool AEat::postPerform()
{
	return true;
}


bool AGetBeer::perform()
{
	this_agent->agentInventory->addItem(new GameObject("Alcohol", "Beer"));

	return true;
}

bool AGetBeer::postPerform()
{
	return true;
}


bool ASleep::perform()
{
	return true;
}


bool ASleep::postPerform()
{
	return true;
}