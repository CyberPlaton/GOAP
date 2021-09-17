#pragma once

#include "BTInterface.h"

class AI
{
public:

	static AI* get()
	{
		if (!g_AI)
		{
			g_AI = new AI();
		}

		return g_AI;
	}

	static void del()
	{
		if (g_AI)
		{
			delete g_AI;
		}
	}

	/*
	* Start update tick for all registered Behvaior Trees.
	*/
	void executeBehaviorTrees() { for (auto& t : behaviorTrees) t->update(); }
	


	void addBehaviorTree(BehaviorTree* tree) { behaviorTrees.push_back(tree); };

	
	
	void setGlobalBlackBoard(BTBlackboard* b) { blackboard = b; }


private:
	AI() {};

	static AI* g_AI;

	BTBlackboard* blackboard = nullptr;
	std::vector<BehaviorTree*> behaviorTrees;
};