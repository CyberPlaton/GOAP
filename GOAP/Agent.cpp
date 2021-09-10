#include "Agent.h"

void Agent::update(double dt)
{
	if (actionStack.empty())
	{
		// Agent has nothing to do.


		// Score needs.


		// Get the need to battle.


		// Search for object that fulfills the need.


		// Push actions on stack:
		// Action associated with the need to fulfill
		// Action to let agent move to fulfilling object


		// return
	}
	else
	{
		// Agent has a goal and set of actions.


		// Get latest action from stack


		// Perform it


		// Postperform and pop it, if it returned true
	}
}