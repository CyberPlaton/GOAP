#pragma once

#include <string>
#include <map>
#include <vector>
#include <typeinfo>


#define DEFAULT_MAPSIZE_X 32
#define DEFAULT_MAPSIZE_Y 32


class Component;
using ComponentMap = std::map<std::string, std::vector<Component*>>;


/*
* The ComponentID is how the user names it, this can be anything, even a unique name for each component,
* for example for the TransformCmp of some Agent named "Ralf" we can assign the ComponentID as := "GO_98_Ralf_Transform".
*/
using ComponentID = std::string;

/*
* The ComponentType is more of a runtime  type information and describes what kind of class the component it,
* thus we can know to what we can cast it. For now it is a string, but will be enumed or hashed...
*/
using ComponentType = std::string;







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
	void add(Component* c, const ComponentType& type);

	/*
	* Each component removes itself on destr.
	*/
	void remove(Component* c);

	/*
	* Iteratable for a set of components of same type.
	*/
	template < class T >
	std::vector<T*> getAllOfType(const ComponentType& type)
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


	bool init(const ComponentType& type)
	{
		// Assign a numerical id to self.
		id = ++g_ComponentID;


		// Store self in storage.
		ComponentStorage::get()->add(this, type);

		return true;
	}


	virtual ComponentType getType() = 0;

	unsigned long long id;
	ComponentID name;
};





class TransformCmp : public Component
{
public:
	TransformCmp(const ComponentID& name) :xpos(0), ypos(0)
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

	ComponentType getType() override { return this->type; }

	ComponentType type;
	int xpos;
	int ypos;
};



class RendererableCmp : public Component
{
public:
	RendererableCmp(const ComponentID& name, float width, float height, const std::string& color):
		width(width), height(height), color(color)
	{
		this->name = name;
		type = "Renderable";

		init(type);
	}


	ComponentType getType() override { return this->type; }




	ComponentType type;

	/*
	* Specify the dimension of it.
	* Where to draw is derived from transform.
	*/
	float width = 0.0f;
	float height = 0.0f;

	std::string color = "none";

	bool render = true;
};