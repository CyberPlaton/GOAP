#include "Actions.h"
#include "Agent.h"


bool ActionMoveToDestination::perform(double dt)
{
	using namespace std;

	// Animate character if needed
	if (getPawn()->hasComponent("Animator"))
	{
		getPawn()->getComponent< AnimatorCmp >("Animator")->setAnimation(AnimatorCmp::Animations::ANIM_WALK);
	}

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
	// Animate character if needed
	if (getPawn()->hasComponent("Animator"))
	{
		getPawn()->getComponent< AnimatorCmp >("Animator")->setAnimation(AnimatorCmp::Animations::ANIM_SLEEP);
	}


	if (timer_started == 0)
	{
		timer->startTimer();
		timer_started = 1;
	}

	// Is the action complete
	double left = getLeftTime();
	if (left > 0.0)
	{
		Agent* pawn = static_cast<Agent*>(getPawn());
		AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");
		GameObject* smartGO = getSmartObject();
		SmartObject* smo = smartGO->getComponent<SmartObject>("SmartObject");
		double a = smo->getFulfillmentAmount("Sleep");

		// Decrease sleep a bit.
		needs->incrementSleep(-(a * GameWorldTime::get()->getTimeSpeed()));


		// Check whether agent does not need sleep anymore.
		double margin = getActionCompletionMargin();
		if (needs->getSleep() <= 0.0 + margin) return true;

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

	cout << "\"ActionSleep\" Action Completed for \"" << this->getPawn()->name << "\"" << endl;


	delete this;
	return true;
}


bool ActionEat::perform(double dt)
{
	using namespace std;

	// Animate character if needed
	if (getPawn()->hasComponent("Animator"))
	{
		getPawn()->getComponent< AnimatorCmp >("Animator")->setAnimation(AnimatorCmp::Animations::ANIM_EAT);
	}

	if (timer_started == 0)
	{
		timer->startTimer();
		timer_started = 1;
	}

	// Get duration of action
	double left = getLeftTime();
	if (left > 0.0)
	{
		Agent* pawn = static_cast<Agent*>(getPawn());
		AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");
		GameObject* smartGO = getSmartObject();
		SmartObject* smo = smartGO->getComponent<SmartObject>("SmartObject");
		double a = smo->getFulfillmentAmount("Hunger");


		cout << color(colors::DARKRED);
		cout << "Decrease Hunger of \""<< this->getPawn()->getName() << "\" by " << -(a * GameWorldTime::get()->getTimeSpeed()) << white << endl;

		// Decrease hunger a bit.
		needs->incrementHunger(-(a * GameWorldTime::get()->getTimeSpeed()));


		// Check whether agent does not need sleep anymore.
		double margin = getActionCompletionMargin();
		if (needs->getHunger() <= 0.0 + margin) return true;

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

	cout << "\"ActionEat\" Action Completed for \"" << this->getPawn()->name << "\"" << endl;

	delete this;
	return true;
}


bool ActionDrink::perform(double dt)
{
	using namespace std;

	// Animate character if needed
	if (getPawn()->hasComponent("Animator"))
	{
		getPawn()->getComponent< AnimatorCmp >("Animator")->setAnimation(AnimatorCmp::Animations::ANIM_DRINK);
	}

	if (timer_started == 0)
	{
		timer->startTimer();
		timer_started = 1;
	}

	// Get duration of action
	double left = getLeftTime();

	if (left > 0.0)
	{
		Agent* pawn = static_cast<Agent*>(getPawn());
		AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");
		GameObject* smartGO = getSmartObject();
		SmartObject* smo = smartGO->getComponent<SmartObject>("SmartObject");
		double a = smo->getFulfillmentAmount("Thirst");

		// Decrease thirst a bit.
		needs->incrementThirst(-(a * GameWorldTime::get()->getTimeSpeed()));


		// Check whether agent does not need sleep anymore.
		double margin = getActionCompletionMargin();
		if (needs->getThirst() <= 0.0 + margin) return true;

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

	cout << "\"ActionDrink\" Action Completed for \"" << this->getPawn()->name << "\"" << endl;

	delete this;
	return true;
}




bool ActionDoNothing::perform(double dt)
{
	using namespace std;

	// Animate character if needed
	if (getPawn()->hasComponent("Animator"))
	{
		getPawn()->getComponent< AnimatorCmp >("Animator")->setAnimation(AnimatorCmp::Animations::ANIM_IDLE);
	}

	if (timer_started == 0)
	{
		timer->startTimer();
		timer_started = 1;

		Agent* pawn = static_cast<Agent*>(getPawn());
		pawn->isIdling = true;

		start_time = timer->currentTime();
	}

	// How much is left to simulate
	double left = getLeftTime();
	if (left > 0.0)
	{
		// Action not completed yet.
		return false;
	}
	else
	{
		return true;
	}
}

bool ActionDoNothing::postPerform(double dt)
{
	using namespace std;


	Agent* pawn = static_cast<Agent*>(getPawn());
	pawn->isIdling = false;

	cout << "\"ActionDoNothing\" Action Completed for \"" << this->getPawn()->name << "\"" << endl;

	delete this;
	return true;
}