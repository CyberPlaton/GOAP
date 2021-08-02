#pragma once


#include <map>
#include <vector>
#include <queue>
#include <tuple>


#include "WorldState.h"
#include "Planner.h"
#include "Action.h"
#include "Timer.h"
#include "Inventory.h"
#include "GameWorldTime.h"
#include "DaySchedule.h"


struct SubGoal
{
	SubGoal(const std::string& key, int value, bool removable) : removable(removable)
	{
		goals.emplace(std::make_pair(value, key));
	}

	// NOTE: The smaller the number, the higher the priority!
	std::map<int, std::string> goals;
	bool removable = true;
};


/*
* Interface class for an agent.
* Note that an agent is a GameObject and must be initialized as such.
*/
class Agent : public GameObject
{
public:


	static void addRoleDefinitionPath(const std::string& role, const std::string& path)
	{
		role_definitions_map.emplace(std::make_pair(role, path));
	}



	Agent(const std::string& tag, const std::string& name) : GameObject(tag, name)
	{
	}

	virtual ~Agent()
	{
	}


	bool assignRole(const std::string& rolename)
	{
		using namespace std;
		using namespace nlohmann;

		std::string defpath = role_definitions_map[rolename];

		ifstream fin(defpath);
		if (fin.is_open() == false) return false;

		json in;
		fin >> in;

		DaySchedule* schedule = new DaySchedule(in.at("ScheduleName").get<std::string>());

		for (auto& e : in.at("Entries"))
		{
			ScheduleEntry* entry = new ScheduleEntry(e.at("ActivityName").get<std::string>(),
													e.at("TargetTag").get<std::string>(),
													e.at("StartTime").get<double>(),
													e.at("EndTime").get<double>());

			schedule->addEntry(entry);
		}

		daySchedule = schedule;
		agentRole = rolename;

		return true;
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

			actionDefinitions.emplace(std::make_pair(name, path));
		}
	}


	/*
	* Called just before the start of the scene.
	*/
	void awake()
	{
		agentInventory = new Inventory();
		agentBeliefs = new WorldStates();
		timer = new HRTimer();
		navigator = new Navigator(this);
	}


	/*
	* Main Update Function.
	*/
	void update()
	{
		// Check for daytime goal transition.
		if (_adjustGoalToDaytime())
		{
			// New Goal!
			delete currentAction;
			currentAction = nullptr;


		}





		// Check whether agent has something to do.
		if (currentAction != nullptr && currentAction->running)
		{
			// Check whether we have reached our destination
			Component* c = getComponent("Transform");
			TransformCmp* t = static_cast<TransformCmp*>(c);
			if (t->xpos == navigator->destx && t->ypos == navigator->desty)
			{
				// Destination reached.
				if (!timerStarted)
				{
					timer->startTimer();
				}


				if (!invoked && timer->getElapsedSeconds() >= currentAction->duration)
				{
					// Call complete function,
					// after given time period has elapsed.
					completeAction();
					invoked = true;
				}
			}

		}

		// Check whether agent is idle.
		if (planner == nullptr || actionQueue.size() == 0)
		{

			planner = new Planner();

			// Goals are already sorted from most important to least important.
			for (auto& sg : goals)
			{
				actionQueue = planner->plan(availableActions, sg.second->goals, agentBeliefs);

				if (actionQueue.size() > 0)
				{
					currentGoal = sg.second;
					break;
				}
			}
		}


		// Check whether we ran out of things to do.
		if (actionQueue.size() > 0)
		{
			if (currentGoal->removable)
			{
				goals = _goalsSubset(goals, currentGoal); // Remove current goal as fulfilled.
			}

			if (planner)
			{
				delete planner;
				planner = nullptr;
			}
		}


		// Check whether we have things to do.
		if (actionQueue.size() > 0)
		{
			currentAction = actionQueue.front();
			actionQueue.pop();

			// Try executing the action.
			if (currentAction->perform())
			{

				// Get the target gameobject if we dont have a valid one.
				if (currentAction->target == nullptr && currentAction->target_tag.compare("") != 0)
				{
					currentAction->target = GameObjectStorage::get()->getGOByTag(currentAction->target_tag);
				}

				// Have we got a valid target?
				if (currentAction->target)
				{
					currentAction->running = true;

					Component* c = currentAction->target->getComponent("Transform");
					TransformCmp* transform = static_cast<TransformCmp*>(c);

					// Set next waypoint destination for agent.
					navigator->setDestination(transform->xpos, transform->ypos);
				}
			}
			else
			{
				// We could not fullfil the action,
				// so clear action queue to get a new one.
				while (actionQueue.empty() == false)
				{
					actionQueue.pop();
				}
			}
		}
	}


	void completeAction()
	{
		currentAction->running = false;
		currentAction->postPerform();
		invoked = false;
	}


	std::map<std::string, std::string> actionDefinitions;
	std::vector<Action*> availableActions;
	std::map<int, SubGoal*> goals;

	std::queue< Action* > actionQueue;
	Action* currentAction = nullptr;

	SubGoal* currentGoal = nullptr;

	bool invoked = false;
	bool timerStarted = false;

	Planner* planner = nullptr;

	WorldStates* agentBeliefs = nullptr;

	HRTimer* timer = nullptr;

	Inventory* agentInventory = nullptr;
	std::vector<GameObject*> agentOwnedObjects;

	Navigator* navigator = nullptr;

	
	/*
	* A role can be given at runtime and defines the daily schedule and behavior.
	*/
	std::string agentRole = "None";

	/*
	* roleGoals are defined as <Goalname, <startDaytime, EndDaytime>>.
	*/
	//std::map<std::string, std::tuple<double, double>> roleGoals;

	/*
	* Schedule of the day for agent.
	* Maps actions to specific daytimes, as well as place of occurance for action.
	*/
	DaySchedule* daySchedule = nullptr;


	static std::map<std::string, std::string> role_definitions_map; // Defines the role name with according path to json definition.

private:
	std::map<int, SubGoal*> _goalsSubset(std::map<int, SubGoal*> goals, SubGoal* exlude)
	{
		std::map<int, SubGoal*> r;

		for (auto& g : goals)
		{
			if (g.second->goals[0].compare(exlude->goals[0]) == 0)
			{
				continue;
			}
			else
			{
				r.emplace(std::make_pair(g.first, g.second));
			}
		}

		return r;
	}




	/*
	* Returns true if goal was changed, else false.
	*/
	bool _adjustGoalToDaytime()
	{
		double daytime = GameWorldTime::get()->getDaytime();

		for (auto& task : daySchedule->schedule)
		{

			double start = task->start;
			double end = task->end;

			// Is it time to pursue the roal goal?
			if (start < daytime && end > daytime)
			{
				// Time to change goal.
				// For now we do this apruptly.

				goals.clear(); // Clear all previous goals.

				SubGoal* new_goal = new SubGoal(task->name, 1, true);
				goals.emplace(std::make_pair(1, new_goal)); // Add a new Goal.
				
				return true;
			}

		}

		return false;
	}




};


std::map<std::string, std::string> Agent::role_definitions_map;