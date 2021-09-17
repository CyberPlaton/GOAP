#pragma once

#include "GameObject.h"
#include "ComponentSystem.h"


/*
* The Inventory stores how many of a certain object an entity has.
* Note that the name of the object should match the definition file,
* in order to get it dynamically as an Gameobject.
*/
class InventoryCmp : public Component
{
public:
	InventoryCmp(const ComponentID& name)
	{
		this->name = name;
		type = "Inventory";
		init(type);
	}

	~InventoryCmp() { inventory.clear(); }

	ComponentType getType() override { return this->type; }


	void addItem(const std::string& name)
	{
		// Increase or emplace a new one.
		inventory[name] = inventory[name] + 1;
	}


	void removeItem(const std::string& name)
	{
		auto it = inventory.find(name);

		if (it != inventory.end())
		{
			if (inventory[name] == 0)
			{
				return;
			}
			else
			{
				inventory[name] -= 1;
			}
		}
	}

	/*
	* Construct dynamically an item from the inventory.
	*/
	GameObject* getItem(const std::string& name);


	std::map<std::string, int >& getItems()
	{
		return inventory;
	}


private:
	ComponentType type;
	
	std::map<std::string, int > inventory;
};