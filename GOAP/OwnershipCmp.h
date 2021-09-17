#pragma once

#include "ComponentSystem.h"
#include "GameObject.h"


/*
* Every Gameobject which can own or can be owned does have this component.
* 
* Thus there is an inherent tree structure, e.g.
* 
* Agent Ralf owns a House and the House own a bed.
* Thus Ralf owns too the bed.
* 
* To get the root owner of the tree, in this case Ralf, we call "getRootGameobject()" with any
* of the components of House, bed or Ralf.
* To get the direct owner of the current object, we call "getOwnerTag()".
* 
*/
class OwnershipCmp : public Component
{
public:
	/*
	* gameobject_tag: The tag of the gameobject to which this component belongs.
	*/
	OwnershipCmp(const ComponentID& name, const GOTag& goTag) : gameobjectTag(goTag)
	{
		this->name = name;
		type = "Ownership";
		init(type);
	}

	std::string getType() override { return this->type; }


	/*
	* Return the gameobject which in the hierarchie does own this gameobject.
	* For example:
	*		table->getOwnerGameobject() -> Ralf.
	*/
	GameObject* getOwnerGameobject()
	{
		if (owned)
		{
			return GameObjectStorage::get()->getGOByTag(ownerGameobjectTag);
		}

		return nullptr;
	}


	/*
	*/
	GameObject* getRootGameobject()
	{
		if (!owned) return GameObjectStorage::get()->getGOByTag(gameobjectTag);
		else
		{
			return getOwnerGameobject()->getComponent<OwnershipCmp>("Ownership")->getRootGameobject();
		}
	}

	GOTag getOwnerTag()
	{
		return ownerGameobjectTag;
	}

	bool isOwned()
	{
		return owned;
	}


	bool doesOwnObjects()
	{
		return owned_objects.size() > 0;
	}

	void setOwner(const GOTag& tag)
	{
		owned = true;
		ownerGameobjectTag = tag;
	}

private:
	std::string type;

	/*
	* Tag of the gameobject, to which this component belongs.
	*/
	GOTag gameobjectTag = "null";


	/*
	* Whether the gameobject with this component is owned by another.
	*/
	bool owned = false;


	/*
	* If the gameobject with this component is owned, then this is the owner objects tag.
	*/
	GOTag ownerGameobjectTag = "null";
	

	/*
	* Tags of gameobjects which the gameobject owns.
	*/
	std::vector<GOTag> owned_objects;
};