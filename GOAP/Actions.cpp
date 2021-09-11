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
		// Check whether agent does not need sleep anymore.
		Agent* pawn = static_cast<Agent*>(getPawn());
		AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");
		if (needs->getSleep() == 0.0) return true;


		// Decrease sleep a bit.
		needs->setSleep(needs->getSleep() - 3.14152);


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

	if (timer->getElapsedGamehours() <= duration) return false;
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

	Agent* pawn = static_cast<Agent*>(getPawn());

	AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");

	GameObject* smartGO = getSmartObject();
	SmartObject* smo = smartGO->getComponent<SmartObject>("SmartObject");


	needs->setThirst(needs->getThirst() - smo->getFulfillmentAmount("Thirst"));

	cout << "\"ActionDrink\" Action Completed for \"" << this->getPawn()->name << "\"" << endl;

	return true;
}