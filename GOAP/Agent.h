#pragma once


#include <map>
#include <vector>
#include <queue>


#include "WorldState.h"
#include "Planner.h"
#include "Action.h"
#include "Timer.h"


struct SubGoal
{
	SubGoal(const std::string& key, int value, bool removable) : removable(removable)
	{
		goals.emplace(std::make_pair(key, value));
	}

	std::map<std::string, int> goals;
	bool removable = true;
};


/*
* Interface class for an agent.
*/
class Agent 
{
public:
	virtual ~Agent()
	{

	}

	/*
	* Specify all available actions for an agent.
	* Here we specify a filepath for a json file from which we derive the path of all actions.
	*/
	bool init(const std::string& filepath)
	{
		using namespace std;
		using namespace nlohmann;

		ifstream fin(filepath);
		if (fin.is_open() == false) return false;

		json in;
		fin >> in;

		for (auto& entry : in.at("Actions"))
		{
			std::string name = entry[0].get<std::string>();
			std::string path = entry[1].get<std::string>();

			availableActions.emplace(std::make_pair(name, path));
		}
	}


	/*
	* Called just before the start of the scene.
	*/
	void awake()
	{

	}


	/*
	* Main Update Function.
	*/
	void update()
	{
		// TODO
	}


	void completeAction()
	{
		currentAction->running = false;
		currentAction->postPerform();
		invoked = false;
	}


private:

	std::map<std::string, std::string> availableActions;

	std::queue< Action* > actionQueue;
	Action* currentAction = nullptr;

	SubGoal* currentGoal = nullptr;

	bool invoked = false;

	Planner* planner = nullptr;

	WorldStates* agentBeliefs = nullptr;

	HRTimer* timer = nullptr;
};