#pragma once

#include <vector>
#include <string>

#include "ComponentSystem.h"

#include "nlohmann/json.hpp"


static std::hash<std::string> hasher;
class GameObject;


class GameObjectStorage
{
public:

	static GameObjectStorage* get();

	static void del();

	std::vector< GameObject* > getStorage();

	void add(GameObject* go);

	GameObject* getGO(const std::string& tag);


private:

	GameObjectStorage(){}

	static GameObjectStorage* g_GameObjectStorage;

	std::vector< GameObject* > m_GameObjects;
};




class GameObject
{
public:
	GameObject(const std::string& tag) : tag(tag)
	{
		hash = hasher(tag);

		GameObjectStorage::get()->add(this);
	}


	void AddComponent(Component* c)
	{
		components.push_back(c);
	}


	void RemoveComponent(Component* c)
	{
		auto it = std::find(components.begin(), components.end(), c);

		if (it != components.end())
		{
			components.erase(it);
		}
	}

	Component* getComponent(const std::string& name)
	{
		for (auto& cmp : components)
		{
			if (cmp->name.compare(name) == 0)
			{
				return cmp;
			}
		}
	}




	void setPosition(int x, int y)
	{
		Component* cmp = this->getComponent(this->tag + "_Transform");

		if (cmp != nullptr)
		{
			TransformCmp* t = static_cast<TransformCmp*>(cmp);

			t->xpos = x;
			t->ypos = y;
		}
	}


	/*
	* Change tag and hash for gameobject. 
	* Those should automatically be adjusted in GOStorage,
	* as there ar just the pointers to objects.
	*/
	void setTag(const std::string& tag)
	{
		this->tag = tag;
		this->hash = hasher(this->tag);
	}


	std::string tag;
	size_t hash;


	std::vector<Component*> components;

};