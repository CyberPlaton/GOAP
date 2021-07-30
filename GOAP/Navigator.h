#pragma once


#include "GameObject.h"


class Navigator
{
public:

	Navigator(GameObject* go) : agent(go)
	{

	}


	/*
	* Set the next waypoint/moving dest. for a gameobject.
	*/
	void setDestination(float x, float y)
	{
		destx = x;
		desty = y;
	}


	/*
	* General update function.
	*/
	void update()
	{
		if (Component* cmp = agent->getComponent("Transform"); cmp != nullptr)
		{
			TransformCmp* t = static_cast<TransformCmp*>(cmp);

			if (t->xpos < destx)
			{
				t->xpos += 0.1f;
			}

			if (t->ypos < desty)
			{
				t->ypos += 0.1f;
			}

			if (t->xpos > destx)
			{
				t->xpos -= 0.1f;
			}

			if (t->ypos > desty)
			{
				t->ypos -= 0.1f;
			}
		}
	}


	GameObject* agent = nullptr;

	float destx = 0.0f;
	float desty = 0.0f;
};