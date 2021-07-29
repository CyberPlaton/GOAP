#pragma once

#include <string>
#include <fstream>


#include "GameObject.h"
#include "WorldState.h"
#include "Navigator.h"

#include "nlohmann/json.hpp"

class Action
{
public:
	virtual ~Action(){}

	/*
	* Get information defined in json for this action and store it in self.
	*/
	bool init(const std::string& filepath)
	{
		using namespace std;
		using namespace nlohmann;

		ifstream fin(filepath);
		if (fin.is_open() == false) return false;

		json in;
		fin >> in;
		
		action_name = in.at("Name").get<std::string>();

		// Get the target and agent tags.
		target_tag = in.at("TargetTag").get<std::string>();
		this_agent_tag = in.at("AgentTag").get<std::string>();

		// Get cost, duration etc.
		cost = in.at("Cost").get<int>();
		duration = in.at("Duration").get<int>();

		// Get preconds and effects and create the agent beliefs.
		for (auto& entry : in.at("Preconditions"))
		{
			std::string cond = entry[0].get<std::string>();
			int val = entry[1].get<int>();

			pre_conditions.push_back(new WorldState(cond, val));
		}

		for (auto& entry : in.at("Effects"))
		{
			std::string eff = entry[0].get<std::string>();
			int val = entry[1].get<int>();

			effects.push_back(new WorldState(eff, val));
		}



	}


	/*
	* Called on initialization of the application,
	* just before the scene is starting.
	*/
	void awake()
	{

		target = GameObjectStorage::get()->getGO(target_tag);
		this_agent = GameObjectStorage::get()->getGO(this_agent_tag);
	}


	/*
	* Is the action achievable by itself.
	*/
	bool isGenerallyAchivable()
	{
		return true;
	}


	/*
	* Is the action achievable given the conditions.
	*/
	bool isAchievableGiven(std::map<std::string, int> conditions)
	{
		bool achievable = false;

		// For each condition...
		for (auto& cond : conditions)
		{
			achievable = false;

			// ... check whether it matches the pre conditions of the action
			for (auto& entry : pre_conditions)
			{
				
				if (entry->key.compare(cond.first) == 0)
				{
					// Current condition can be achieved...
					achievable = true;
				}
			}

			// If current condition can not be achieved, then return false
			if (!achievable) return false;
		}

		// If we are here, then all condition can be achieved.
		return true;
	}



	/*
	* Definitions for what the action should actually do.
	*/

	// Perform the action.
	virtual bool perform() = 0;

	// What happens after the action is successfully achieved.
	virtual bool postPerform() = 0;


public:

	std::string action_name;
	float cost = 0.0f;
	GameObject* target = nullptr; // Representing where the action is taking place.
	std::string target_tag;
	float duration = 0.0f; // How long the action needs to be performed.

	
	std::vector< WorldState* > pre_conditions; // Requirements for the action.
	std::vector< WorldState* > effects;		  // Effects of the action.

	WorldStates* agentBeliefs = nullptr;

	bool running = false;

	GameObject* this_agent = nullptr;
	std::string this_agent_tag;
};