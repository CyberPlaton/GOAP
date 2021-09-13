#pragma once

#include "Action.h"
#include "GameWorldTimer.h"

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
		timer = new GameworldTimer();
	}


	bool perform(double dt) override final;
	bool postPerform(double dt) override final;


	int timer_started = 0;
	GameworldTimer* timer = nullptr;
};



class ActionEat : public ActionInstance
{
public:
	ActionEat(ActionDefinition* def, GameObject* pawn, GameObject* smartObject) : ActionInstance(def, pawn, smartObject)
	{
		timer = new GameworldTimer();
	}


	bool perform(double dt) override final;
	bool postPerform(double dt) override final;



	int timer_started = 0;
	GameworldTimer* timer = nullptr;
};



class ActionDrink : public ActionInstance
{
public:
	ActionDrink(ActionDefinition* def, GameObject* pawn, GameObject* smartObject) : ActionInstance(def, pawn, smartObject)
	{
		timer = new GameworldTimer();
	}


	bool perform(double dt) override final;

	bool postPerform(double dt) override final;



	int timer_started = 0;
	GameworldTimer* timer = nullptr;
};



class ActionDoNothing : public ActionInstance
{
public:
	ActionDoNothing(ActionDefinition* def, GameObject* pawn, GameObject* smartObject) : ActionInstance(def, pawn, smartObject)
	{
		timer = new GameworldTimer();
	}


	bool perform(double dt) override final;

	bool postPerform(double dt) override final;

	double getStartTime() override final
	{
		return start_time;
	}

	double getEndTime() override final
	{
		double d = (getStartTime() + getDuration());

		// Mod 24
		if (d > 24.0)
		{
			d = d - 24.0;
		}
		
		return d;
	}

	double getLeftTime() override final
	{
		using namespace std;

		double d = (double)INT_MAX;
		double starttime = getStartTime();
		double duration = getDuration();

		if (starttime + duration > 24.0)
		{
			// Waittime end on next day

			// How long until end of day
			double a = 24.0 - starttime;

			// How long from next day start until end of wait time
			double b = getEndTime();

			// How long the whole waittime
			d = a + b;
		}
		else if (starttime + duration <= 24.0)
		{
			// Waittime end on same day

			// Compute diference between end and start
			d = getEndTime() - starttime;
		}
		
		// How much left from whole waittime
		double left = d - timer->getElapsedGamehours();

		return left;
	}



	int timer_started = 0;
	GameworldTimer* timer = nullptr;


	double start_time = 0.0;
};
