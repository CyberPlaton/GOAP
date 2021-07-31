#pragma once

#include <map>
#include <queue>

#include "Action.h"
#include "GameWorld.h"

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

	/*
	* If no path is found, returns an empty action queue.
	*/
	std::queue<Action*> plan(std::vector<Action*> actions, std::map<int, std::string> goals, WorldStates* beliefs)
	{
		std::queue<Action*> retActions;

		std::vector<Action*> usableActions;

		for (auto& a : actions)
		{
			if (a->isGenerallyAchivable())
			{
				usableActions.push_back(a);
			}
		}



		std::vector<Node*> leaves;

		std::map< std::string, int > worldState = GameWorld::get()->getWorldState()->getStates();

		Node* start = new Node(nullptr, 0.0f, worldState, beliefs->getStates(), nullptr);

		bool success = _buildGraph(start, leaves, usableActions, goals);

		if (!success)
		{
			// Return empty queue.
			return retActions;
		}


		Node* cheapest = nullptr;

		for (auto& leaf : leaves)
		{
			if (cheapest == nullptr)
			{
				cheapest = leaf;
			}
			else
			{
				if (cheapest->cost > leaf->cost)
				{
					cheapest = leaf;
				}
			}
		}


		// Resulting plan.
		std::vector<Action*> r;
		Node* n = cheapest;
		while (n != nullptr)
		{
			if (n->action != nullptr)
			{
				r.insert(r.begin(), n->action);
			}

			n = n->parent;
		}


		for (auto& a : r)
		{
			retActions.push(a);
		}


		return retActions;
	}



private:

	bool _buildGraph(Node* parent, std::vector<Node*> leaves, std::vector<Action*> usableActions, std::map<int, std::string > goal)
	{
		bool foundpath = false;

		for (auto& action : usableActions)
		{
			if (action->isAchievableGiven(parent->state))
			{

				std::map<std::string, int> curr_state;

				for (auto& effect : action->effects)
				{
					if (curr_state.contains(effect->key) == false)
					{
						curr_state.emplace(std::make_pair(effect->key, effect->value));
					}
				}



				Node *node = new Node(parent, parent->cost + action->cost, curr_state, action);


				if (_goalAchieved(goal, curr_state))
				{
					leaves.push_back(node);
					foundpath = true;
				}
				else
				{
					std::vector<Action*> subset = _actionSubset(usableActions, action);

					// Go into recursion.
					bool found = _buildGraph(node, leaves, subset, goal);

					if (found)
					{
						foundpath = true;
					}
				}

			}

		}


		return foundpath;
	}


	bool _goalAchieved(std::map<int, std::string> goal, std::map<std::string, int> state)
	{
		for (auto& g : goal)
		{
			if (!state.contains(g.second))
			{
				return false;
			}
		}

		return true;
	}

	/*
	* 
	*/
	std::vector<Action*> _actionSubset(std::vector<Action*> vec, Action* a)
	{
		std::vector<Action*> new_vec;

		for (auto& e : vec)
		{
			if (e->action_name.compare(a->action_name) == 0)
			{
				continue;
			}

			new_vec.push_back(e);
		}

		return new_vec;
	}

};
