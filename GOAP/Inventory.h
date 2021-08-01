#pragma once

#include "GameObject.h"



class Inventory
{
public:
	Inventory(){}


	void addItem(GameObject* go)
	{
		inventory.push_back(go);
	}


	void removeItem(GameObject* go)
	{
		auto it = std::find(inventory.begin(), inventory.end(), go);

		if (it != inventory.end())
		{
			inventory.erase(it);
		}
	}

	GameObject* getItem(const std::string& tag)
	{
		for (auto& obj : inventory)
		{
			if (obj->tag.compare(tag) == 0)
			{
				return obj;
			}
		}

		return nullptr;
	}


	std::vector<GameObject*> getItems()
	{
		return inventory;
	}


private:

	std::vector<GameObject*> inventory;
};