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

	void executeBehaviorTrees() { for (auto& t : behaviorTrees) t->update(); }
	void addBehaviorTree(BehaviorTree* tree) { behaviorTrees.push_back(tree); };


private:
	AI() {};

	static AI* g_AI;

	std::vector<BehaviorTree*> behaviorTrees;
};