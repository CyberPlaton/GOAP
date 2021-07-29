#pragma once

#include <map>
#include <queue>

#include "Action.h"


struct Node
{
	Node(Node* parent, float cost, std::map<std::string, int> all_states, Action* action):
		parent(parent), cost(cost), action(action), state(all_states)
	{

	}


	Node(Node* parent, float cost, std::map<std::string, int> all_states, std::map<std::string, int> belief_states, Action* action):
		parent(parent), cost(cost), action(action), state(all_states)
	{
		// Add other states from beliefs which are not in all states.
		for (auto& s : all_states)
		{
			if (state.contains(s.first) == false)
			{
				state.emplace(std::make_pair(s.first, s.second));
			}
		}
	}

	Node* parent = nullptr;
	float cost = 0.0f;
	std::map<std::string, int> state;
	Action* action = nullptr;
};




class Planner
{
public:

	std::queue<Action*> plan(std::vector<Action*> actions, std::map<std::string, int> goals, WorldStates* beliefs)
	{

	}



private:

	bool _buildGraph()
	{

	}

	bool _goalAchieved()
	{

	}

	/*
	* 
	*/
	std::vector<Action*> _actionSubset()
	{

	}

};
