#pragma once
#include "Action.h"

#include <iostream>


class ActionMoveToDestination : public ActionInstance
{
public:
	ActionMoveToDestination(ActionDefinition* def, GameObject* pawn, int destx, int desty) : ActionInstance(def, pawn), destx(destx), desty(desty)
	{

	}


	bool perform(double dt) override final
	{
		using namespace std;

		cout << "\"MoveToDestinationAction\" Action Completed for \"" << this->getPawn()->name << "\" reaching target at {"<< destx << ":"<< desty << "}" << endl;

		return true;
	}

	bool postPerform(double dt) override final
	{
		return true;
	}



	int destx, desty;
};



class ActionSleep : public ActionInstance
{
public:
	ActionSleep(ActionDefinition* def, GameObject* pawn) : ActionInstance(def, pawn)
	{

	}


	bool perform(double dt) override final
	{
		using namespace std;

		cout << "\"ActionSleep\" Action Completed for \"" << this->getPawn()->name << "\"" << endl;

		return true;
	}

	bool postPerform(double dt) override final
	{
		return true;
	}
};



class ActionEat : public ActionInstance
{
public:
	ActionEat(ActionDefinition* def, GameObject* pawn) : ActionInstance(def, pawn)
	{

	}


	bool perform(double dt) override final
	{
		using namespace std;

		cout << "\"ActionEat\" Action Completed for \"" << this->getPawn()->name << "\"" << endl;

		return true;
	}

	bool postPerform(double dt) override final
	{
		return true;
	}
};