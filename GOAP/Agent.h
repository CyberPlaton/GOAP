#pragma once

#include "ComponentSystem.h"
#include "Components.h"
#include "GameObject.h"

#include "Inventory.h"
#include "WorldState.h"
#include "DaySchedule.h"


/*
* An agent is an extended gameobject with game specific functionality.
*/
class Agent : public GameObject
{
public:
	Agent(const std::string& tag, const std::string& name) : GameObject(tag, name)
	{
	}




private:


	Inventory* agentInventory = nullptr;
	std::vector<GameObject*> agentOwnedObjects;
};