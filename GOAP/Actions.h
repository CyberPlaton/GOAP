#pragma once

#include "Action.h"

#include <iostream>


class ActionMoveToDestination : public ActionInstance
{
public:
	ActionMoveToDestination(ActionDefinition* def, GameObject* pawn, GameObject* smartObject = nullptr, int destx = -1, int desty = -1) : ActionInstance(def, pawn, smartObject), destx(destx), desty(desty)
	{

	}


	bool perform(double dt) override final;

	bool postPerform(double dt) override final
	{
		delete this;
		return true;
	}


	bool destinationAssigned = false;
	int destx, desty;
};



class ActionSleep : public ActionInstance
{
public:
	ActionSleep(ActionDefinition* def, GameObject* pawn, GameObject* smartObject) : ActionInstance(def, pawn, smartObject)
	{

	}


	bool perform(double dt) override final;

	bool postPerform(double dt) override final
	{
		return true;
	}
};



class ActionEat : public ActionInstance
{
public:
	ActionEat(ActionDefinition* def, GameObject* pawn, GameObject* smartObject) : ActionInstance(def, pawn, smartObject)
	{

	}


	bool perform(double dt) override final;

	bool postPerform(double dt) override final
	{
		return true;
	}
};



class ActionDrink : public ActionInstance
{
public:
	ActionDrink(ActionDefinition* def, GameObject* pawn, GameObject* smartObject) : ActionInstance(def, pawn, smartObject)
	{

	}


	bool perform(double dt) override final;

	bool postPerform(double dt) override final
	{
		return true;
	}
};
