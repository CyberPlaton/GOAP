#include "ComponentSystem.h"

ComponentStorage* ComponentStorage::g_ComponentStorageStorage = nullptr;
unsigned long long Component::g_ComponentID = 0;

ComponentStorage* ComponentStorage::get()
{
	if (!g_ComponentStorageStorage)
	{
		g_ComponentStorageStorage = new ComponentStorage();
	}

	return g_ComponentStorageStorage;
}

void ComponentStorage::del()
{
	if (g_ComponentStorageStorage)
	{
		delete g_ComponentStorageStorage;
	}
}



void ComponentStorage::add(Component* c, const std::string& type)
{
	if (componentMap.contains(type))
	{
		componentMap.at(type).push_back(c);
	}
	else
	{
		std::vector<Component*> vec; vec.push_back(c);
		componentMap.emplace(std::make_pair(type, vec));
	}
}


void ComponentStorage::remove(Component* c)
{
	if (componentMap.contains(c->getType()))
	{
		std::vector<Component*>& vec = componentMap.at(c->getType());
		
		auto it = std::find(vec.begin(), vec.end(), c);
		
		if (it != vec.end())
		{
			vec.erase(it);
		}
	}
	else
	{
		delete c; // Maybe unsafe.
	}
}