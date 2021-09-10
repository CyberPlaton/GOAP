#pragma once

#include "ComponentSystem.h"
#include "Components.h"
#include "GameObject.h"
#include "SmartObject.h"
#include "OwnershipCmp.h"

#include "Inventory.h"
#include "WorldState.h"
#include "DaySchedule.h"

#include "AgentNeeds.h"
#include "AgentRole.h"
#include "AgentStats.h"

#include "Action.h"

/*
* An agent is an extended gameobject with game specific functionality.
*/
class Agent : public GameObject
{
public:
	Agent(const std::string& tag, const std::string& name) : GameObject(tag, name)
	{
		agentInventory = new Inventory();
		AddComponent(new AgentStatsCmp(name + "_Stats"));
		AddComponent(new AgentNeedsCmp(name + "_Needs"));
		//AddComponent(new AgentRoleCmp("Role", "None"));
	}

	void update(double dt);




private:

	std::map< std::string, double > needsScoreMap;
	std::stack< ActionInstance* > actionStack;
	Inventory* agentInventory = nullptr;
	std::vector<GameObject*> agentOwnedObjects;


private:
	/*
	* Use scoring functions to score the needs in the map.
	*/
	void scoreNeeds();

	/*
	* Use a function to get the need we want currently to battle.
	*/
	std::string getNeedToBattle();

	/*
	* Search for object which does satisfy the need.
	* Searching is done in owned object or inventory and then in the world around.
	*/
	GameObject* getObjectToFulfillNeedWith(const std::string& need);

};