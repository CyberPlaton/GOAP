#include "GameObject.h"

GameObjectStorage* GameObjectStorage::g_GameObjectStorage = nullptr;



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

std::vector< GameObject* > GameObjectStorage::getStorage()
{
	return g_GameObjectStorage->m_GameObjects;
}

void GameObjectStorage::add(GameObject* go)
{
	m_GameObjects.push_back(go);
}


GameObject* GameObjectStorage::getGO(const std::string& tag)
{
	size_t tag_hash = hasher(tag);

	for (auto& go : m_GameObjects)
	{
		if (tag_hash == go->hash) return go;
	}

	return nullptr;
}