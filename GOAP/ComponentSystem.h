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
		type = "Component";


		// Store self in storage.
		ComponentStorage::get()->add(this);

		return true;
	}

	std::string type;
	unsigned long long id;
	std::string name;
};





class TransformCmp : public Component
{
public:
	TransformCmp(const std::string& name) :xpos(0), ypos(0)
	{
		init();
		this->name = name;
		type = "Transform";
	}


	void setPosition(int x, int y)
	{
		this->xpos = x;
		this->ypos = y;
	}

	int xpos;
	int ypos;
};



class RendererableCmp : public Component
{
public:
	RendererableCmp(const std::string& name, float width, float height, const std::string& color):
		width(width), height(height), color(color)
	{
		init();
		this->name = name;
		this->type = "Renderable";
	}


	/*
	* Specify the dimension of it.
	* Where to draw is derived from transform.
	*/
	float width = 0.0f;
	float height = 0.0f;

	std::string color = "none";

	bool render = true;
};