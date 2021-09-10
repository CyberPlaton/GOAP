#include "Actions.h"
#include "Agent.h"


bool ActionMoveToDestination::perform(double dt)
{
	using namespace std;

	cout << "\"MoveToDestinationAction\" Action Completed for \"" << this->getPawn()->name << "\" reaching target at {" << destx << ":" << desty << "}" << endl;

	return true;
}