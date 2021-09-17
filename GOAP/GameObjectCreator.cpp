#include "GameObjectCreator.h"


GameObject* InventoryCmp::getItem(const std::string& name)
{
	for (auto& obj : inventory)
	{
		if (obj.first.compare(name) == 0)
		{
			std::string path = "GOAP/Gameobjects/" + name + ".xml";

			// Construct the gameobject dynamically and return it.
			GameObjectCreator creator;
			return creator.create(path, name, 0, 0);
		}
	}

	return nullptr;
}