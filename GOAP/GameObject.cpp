#include "GameObject.h"

GameObjectStorage* GameObjectStorage::g_GameObjectStorage = nullptr;
unsigned long long GameObject::g_GameObjectCount = 0;


GameObjectStorage* GameObjectStorage::get()
{
	if (!g_GameObjectStorage)
	{
		g_GameObjectStorage = new GameObjectStorage();
	}

	return g_GameObjectStorage;
}

void GameObjectStorage::del()
{
	if (g_GameObjectStorage)
	{
		g_GameObjectStorage->m_GameObjects.clear();
		delete g_GameObjectStorage;
	}
}

std::vector< GameObject* >& GameObjectStorage::getStorage()
{
	return g_GameObjectStorage->m_GameObjects;
}

void GameObjectStorage::add(GameObject* go)
{
	m_GameObjects.push_back(go);
}


GameObject* GameObjectStorage::getGOByTag(const GOTag& tag)
{
	size_t tag_hash = hasher(tag);

	for (auto& go : m_GameObjects)
	{
		if (tag_hash == go->hash) return go;
	}

	return nullptr;
}


GameObject* GameObjectStorage::findAnyGOByTag(const GOTag& tag)
{
	for (auto& go : m_GameObjects)
	{
		if (go->getTag().find(tag) != std::string::npos)
		{
			return go;
		}
	}

	return nullptr;
}



GameObject* GameObjectStorage::findAnyGOByName(const GOName& name)
{
	for (auto& go : m_GameObjects)
	{
		if (go->getName().find(name) != std::string::npos)
		{
			return go;
		}
	}

	return nullptr;
}


GameObject* GameObjectStorage::getGOByName(const GOName& name)
{
	for (auto& go : m_GameObjects)
	{
		if (go->getName().compare(name) == 0)
		{
			return go;
		}
	}

	return nullptr;
}