#pragma once

#include <vector>
#include <string>

#include "ComponentSystem.h"

#include "common/include/nlohmann/json.hpp"

/*
* The GOTag can be seen as the App unique identifier for a the GameObject.
* Thus we can identify each and we can easily search for each.
* 
* The tag is generated automatically on construction like for example "GO_16_Furniture".
*/
using GOTag = std::string;

/*
* THE GOName is some given or generated name for a gameobject, which is not necesseraly
* unique and can be duplicate, like the name "John" in a city.
*/
using GOName = std::string;


static std::hash<std::string> hasher;
class GameObject;


class GameObjectStorage
{
public:

	static GameObjectStorage* get();

	static void del();

	std::vector< GameObject* >& getStorage();

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
	* GOTag is a GUID for a GameObject.
	* GOName is just a name and can be duplicate.
	*/
	GameObject(const GOTag& tag, const GOName& name) : name(name)
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

	Component* getComponent(const ComponentType& name)
	{
		for (auto& cmp : components)
		{
			// All components are unique for a GameObject,
			// so there cannot be e.g. 2 Transforms.. Thus we can search with ComponentType.

			if (cmp->getType().compare(name) == 0)
			{
				return cmp;
			}

			/*
			// Search for component adjusted to own name.
			if (cmp->name.compare(name) == 0)
			{
				return cmp;
			}
			*/
		}

		return nullptr;
	}


	template < typename T >
	T* getComponent(const std::string& name)
	{
		return static_cast<T*>(getComponent(name));
	}



	bool hasComponent(const ComponentType& name)
	{
		for (auto& cmp : components)
		{

			if (cmp->getType().compare(name) == 0)
			{
				return true;
			}

			/*
			// Search for component adjusted to own name.
			if (cmp->name.compare(name) == 0)
			{
				return true;
			}
			*/
		}

		return false;
	}


	virtual void update(double dt) { return; }


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



