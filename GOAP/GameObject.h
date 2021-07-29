#pragma once

#include <vector>
#include <string>


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




	std::string tag;
	size_t hash;
};