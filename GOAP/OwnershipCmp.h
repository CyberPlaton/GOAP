#pragma once

#include "ComponentSystem.h"
#include "GameObject.h"


class OwnershipCmp : public Component
{
public:
	OwnershipCmp()
	{
		this->name = name;
		type = "Ownership";
		init(type);
	}

	std::string getType() override { return this->type; }

	void setOwner(GameObject* go)
	{
		if (go)
		{
			owner_name = go->name;
			owner_tag = go->tag;
			owned = true;
		}
		else
		{
			owner_name = "";
			owner_tag = "";
			owned = false;
		}
	}



	bool owned = false;
	std::string owner_name;
	std::string owner_tag;

private:
	std::string type;
};