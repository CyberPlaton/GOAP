#include "Actions.h"
#include "Agent.h"


bool ActionMoveToDestination::perform(double dt)
{
	using namespace std;

	// Check if waypoint was assigned
	if (destx == -1 || desty == -1)
	{
		return false;
	}
	else if(!destinationAssigned)
	{
		getPawn()->getComponent<NavigatorCmp>("Navigator")->setDestination(destx, desty);
		destinationAssigned = true;
	}


	// Update movement and 
	// Check if target was reached
	if (getPawn()->getComponent<NavigatorCmp>("Navigator")->update(dt))
	{
		cout << "\"MoveToDestinationAction\" Action Completed for \"" << this->getPawn()->name << "\" reaching target at {" << destx << ":" << desty << "}" << endl;
		
		destx = -1;
		desty = -1;

		return true;
	}


	return false;
}



bool ActionSleep::perform(double dt)
{
	if (timer_started == 0)
	{
		timer->startTimer();
		timer_started = 1;
	}

	// Get duration of action ( as mean )
	double duration = (getMaxDuration() + getMinDuration()) / 2.0;

	if (timer->getElapsedGamehours() <= duration)
	{
		Agent* pawn = static_cast<Agent*>(getPawn());
		AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");

		// Decrease sleep a bit.
		needs->incrementSleep(-(1.3 * GameWorldTime::get()->getTimeSpeed()));

		// Check whether agent does not need sleep anymore.
		if (needs->getSleep() <= 0.0) return true;

		// Action not completed yet.
		return false;
	}
	else
	{
		return true;
	}
}

bool ActionSleep::postPerform(double dt)
{
	using namespace std;


	Agent* pawn = static_cast<Agent*>(getPawn());

	AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");

	GameObject* smartGO = getSmartObject();
	SmartObject* smo = smartGO->getComponent<SmartObject>("SmartObject");


	needs->setSleep(needs->getSleep() - smo->getFulfillmentAmount("Sleep"));


	cout << "\"ActionSleep\" Action Completed for \"" << this->getPawn()->name << "\"" << endl;



	delete this;
	return true;
}


bool ActionEat::perform(double dt)
{
	using namespace std;

	if (timer_started == 0)
	{
		timer->startTimer();
		timer_started = 1;
	}

	// Get duration of action
	double duration = getMaxDuration() - getMinDuration();

	if (timer->getElapsedGamehours() <= duration)
	{
		Agent* pawn = static_cast<Agent*>(getPawn());
		AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");

		// Decrease hunger a bit.
		needs->incrementHunger(-(1.3 * GameWorldTime::get()->getTimeSpeed()));

		// Check whether agent does not need sleep anymore.
		if (needs->getHunger() <= 0.0) return true;

		// Action not completed yet.
		return false;
	}
	else
	{
		return true;
	}
}

bool ActionEat::postPerform(double dt)
{
	using namespace std;


	Agent* pawn = static_cast<Agent*>(getPawn());

	AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");

	GameObject* smartGO = getSmartObject();
	SmartObject* smo = smartGO->getComponent<SmartObject>("SmartObject");


	needs->setHunger(needs->getHunger() - smo->getFulfillmentAmount("Hunger"));

	cout << "\"ActionEat\" Action Completed for \"" << this->getPawn()->name << "\"" << endl;


	delete this;
	return true;
}


bool ActionDrink::perform(double dt)
{
	using namespace std;

	if (timer_started == 0)
	{
		timer->startTimer();
		timer_started = 1;
	}

	// Get duration of action
	double duration = getMaxDuration() - getMinDuration();

	if (timer->getElapsedGamehours() <= duration)
	{
		Agent* pawn = static_cast<Agent*>(getPawn());
		AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");

		// Decrease thirst a bit.
		needs->incrementThirst(-(1.3 * GameWorldTime::get()->getTimeSpeed()));

		// Check whether agent does not need sleep anymore.
		if (needs->getThirst() <= 0.0) return true;

		// Action not completed yet.
		return false;
	}
	else
	{
		return true;
	}
}

bool ActionDrink::postPerform(double dt)
{
	using namespace std;

	Agent* pawn = static_cast<Agent*>(getPawn());

	AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");

	GameObject* smartGO = getSmartObject();
	SmartObject* smo = smartGO->getComponent<SmartObject>("SmartObject");


	needs->setThirst(needs->getThirst() - smo->getFulfillmentAmount("Thirst"));

	cout << "\"ActionDrink\" Action Completed for \"" << this->getPawn()->name << "\"" << endl;


	delete this;
	return true;
}