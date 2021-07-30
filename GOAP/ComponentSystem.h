#pragma once

#include <string>
#include <map>
#include <vector>
#include <typeinfo>


class Component;
using ComponentMap = std::map<std::string, std::vector<Component*>>;

/*
* General storage for all components with types.
* Usage, e.g. for iteration of all components with some type.
*/
class ComponentStorage
{
public:

	static ComponentStorage* get();

	static void del();

	/*
	* Each comonent adds itself on creation.
	*/
	void add(Component* c);

	/*
	* Each component removes itself on destr.
	*/
	void remove(Component* c);

	/*
	* Iteratable for a set of components of same type.
	*/
	std::vector<Component*>& getAllOfType(const std::string& type);

private:

	ComponentStorage() {}

	static ComponentStorage* g_ComponentStorageStorage;

	ComponentMap componentMap;
};




class Component
{
private:

	static unsigned long long g_ComponentID;

public:

	virtual ~Component(){}


	bool init()
	{
		// Assign a numerical id to self.
		id = ++g_ComponentID;


		// Get type name.
		type = typeid(this).name();


		// Store self in storage.
		ComponentStorage::get()->add(this);
	}

	std::string type;
	unsigned long long id;
	std::string name;
};





class TransformCmp : public Component
{
public:
	TransformCmp(const std::string& name) :xpos(0.0f), ypos(0.0f)
	{
		init();
		this->name = name;
	}



	float xpos;
	float ypos;
};