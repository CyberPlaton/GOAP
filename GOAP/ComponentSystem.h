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
	void add(Component* c, const std::string& type);

	/*
	* Each component removes itself on destr.
	*/
	void remove(Component* c);

	/*
	* Iteratable for a set of components of same type.
	*/
	template < class T >
	std::vector<T*> getAllOfType(const std::string& type)
	{
		if (componentMap.contains(type))
		{
			std::vector<T*> r;

			for (auto& c : componentMap.at(type))
			{
				r.push_back(static_cast<T*>(c));
			}

			return r;
		}
	}

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


	bool init(const std::string& type)
	{
		// Assign a numerical id to self.
		id = ++g_ComponentID;


		// Store self in storage.
		ComponentStorage::get()->add(this, type);

		return true;
	}


	virtual std::string getType() = 0;

	unsigned long long id;
	std::string name;
};





class TransformCmp : public Component
{
public:
	TransformCmp(const std::string& name) :xpos(0), ypos(0)
	{
		this->name = name;
		type = "Transform";

		init(type);
	}


	void setPosition(int x, int y)
	{
		this->xpos = x;
		this->ypos = y;
	}

	std::string getType() override { return this->type; }

	std::string type;
	int xpos;
	int ypos;
};



class RendererableCmp : public Component
{
public:
	RendererableCmp(const std::string& name, float width, float height, const std::string& color):
		width(width), height(height), color(color)
	{
		this->name = name;
		type = "Renderable";

		init(type);
	}


	std::string getType() override { return this->type; }

	/*
	* Specify the dimension of it.
	* Where to draw is derived from transform.
	*/
	std::string type;

	float width = 0.0f;
	float height = 0.0f;

	std::string color = "none";

	bool render = true;
};