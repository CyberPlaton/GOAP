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

	GameObject* getGOByTag(const std::string& tag);
	GameObject* findAnyGOByTag(const std::string& tag);
	GameObject* getGOByName(const std::string& name);


private:

	GameObjectStorage(){}

	static GameObjectStorage* g_GameObjectStorage;

	std::vector< GameObject* > m_GameObjects;
};




class GameObject
{
public:
	/*
	* Construct a gameobject.
	* 
	* A Tag is the string identifier and is unique for each gameobject.
	* A Name is the user defined "special" name, e.g. the name for an NPC,
	* and it can be duplicate.
	*/
	GameObject(const std::string& tag, const std::string& name) : name(name)
	{
		this->tag = "GO_" + std::to_string(++g_GameObjectCount) + "_" + tag;

		hash = hasher(this->tag);

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
			// Search for component adjusted to own name.
			if (cmp->name.compare(tag + "_" + name) == 0)
			{
				return cmp;
			}
		}

		return nullptr;
	}


	bool hasComponent(const std::string& name)
	{
		for (auto& cmp : components)
		{
			// Search for component adjusted to own name.
			if (cmp->name.compare(tag + "_" + name) == 0)
			{
				return true;
			}
		}

		return false;
	}



	void setPosition(int x, int y)
	{
		Component* cmp = this->getComponent("Transform");

		if (cmp != nullptr)
		{
			TransformCmp* t = static_cast<TransformCmp*>(cmp);
			t->setPosition(x, y);
		}
	}


	/*
	* Change tag and hash for gameobject. 
	* Those should automatically be adjusted in GOStorage,
	* as there are just the pointers to objects.
	*/
	void setTag(const std::string& tag)
	{
		this->tag = tag;
		this->hash = hasher(this->tag);
	}

	std::string getTag() { return tag; }
	std::string getName() { return name; }


	std::string name;
	std::string tag;
	size_t hash;


	std::vector<Component*> components;

	static unsigned long long g_GameObjectCount;
};



