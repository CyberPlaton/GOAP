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
	using namespace std;


	Agent* pawn = static_cast<Agent*>(getPawn());

	AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");

	GameObject* smartGO = getSmartObject();
	SmartObject* smo = smartGO->getComponent<SmartObject>("SmartObject");


	needs->setSleep(needs->getSleep() - smo->getFulfillmentAmount("Sleep"));


	cout << "\"ActionSleep\" Action Completed for \"" << this->getPawn()->name << "\"" << endl;

	return true;
}



bool ActionEat::perform(double dt)
{
	using namespace std;

	Agent* pawn = static_cast<Agent*>(getPawn());

	AgentNeedsCmp* needs = pawn->getComponent<AgentNeedsCmp>("AgentNeeds");

	GameObject* smartGO = getSmartObject();
	SmartObject* smo = smartGO->getComponent<SmartObject>("SmartObject");


	needs->setHunger(needs->getHunger() - smo->getFulfillmentAmount("Hunger"));

	cout << "\"ActionEat\" Action Completed for \"" << this->getPawn()->name << "\"" << endl;

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