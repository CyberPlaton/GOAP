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
	void update(float dt)
	{
		if (Component* cmp = agent->getComponent("Transform"); cmp != nullptr)
		{
			TransformCmp* t = static_cast<TransformCmp*>(cmp);

			// Internal update.
			if (t->xpos < destx)
			{
				internal_agent_positionx += dt;
			}
			else if (t->xpos > destx)
			{
				internal_agent_positionx -= dt;
			}


			if (t->ypos < desty)
			{
				internal_agent_positiony += dt;
			}
			else if (t->ypos > desty)
			{
				internal_agent_positiony -= dt;
			}



			// Component update, if needed.
			if (internal_agent_positionx > 1.0f)
			{
				t->xpos += 1;
				internal_agent_positionx = 0.0f;
			}
			else if (internal_agent_positionx < -1.0f)
			{
				t->xpos -= 1;
				internal_agent_positionx = 0.0f;
			}


			if (internal_agent_positiony > 1.0f)
			{
				t->ypos += 1;
				internal_agent_positiony = 0.0f;
			}
			else if (internal_agent_positiony < -1.0f)
			{
				t->ypos -= 1;
				internal_agent_positiony = 0.0f;
			}


		}
	}


	GameObject* agent = nullptr;

	float destx = 0.0f;
	float desty = 0.0f;

	/*
	* Used to count in floats the agents position,
	* and then update the actuall transform component.
	*/
	float internal_agent_positionx = 0.0f;
	float internal_agent_positiony = 0.0f;
};