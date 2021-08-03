#pragma once


#include <map>
#include <vector>
#include <queue>
#include <tuple>

#include "ColorConsole.h"

#include "WorldState.h"
#include "Planner.h"
#include "Action.h"
#include "Timer.h"
#include "Inventory.h"
#include "GameWorldTime.h"
#include "DaySchedule.h"

// Actions
#include "Sleep.h"
#include "Eat.h"
#include "GetBeer.h"


struct SubGoal
{
	SubGoal(const std::string& key, int value, bool removable) : removable(removable)
	{
		goals.emplace(std::make_pair(value, key));
	}

	/*
	* Returns the name of the first subgoal.
	* Note that some goals could have more than one subgoal.
	*/
	std::string getName()
	{
		for (auto& it : goals)
		{
			return it.second;
		}
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
	bool init(const std::string& filepath);


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
			else // Destination not reached, update the navigator.
			{
				navigator->update(walking_speed);
			}
		}


		// Check whether we have things to do, but havent started yet.
		if (actionQueue.size() > 0)
		{
			currentAction = actionQueue.front();

			// Try executing the action.
			if (currentAction->perform())
			{

				// Get the target gameobject if we dont have a valid one.
				if (currentAction->target == nullptr && currentAction->target_name.compare("") != 0)
				{
					currentAction->target = GameObjectStorage::get()->getGOByTag(currentAction->target_name);
				}

				// Have we got a valid target?
				if (currentAction->target)
				{
					currentAction->running = true;

					Component* c = currentAction->target->getComponent("Transform");
					TransformCmp* transform = static_cast<TransformCmp*>(c);

					// Set next waypoint destination for agent.
					navigator->setDestination(transform->xpos, transform->ypos);


					actionQueue.pop();
					return;
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


		// Check whether agent is idle.
		if (planner == nullptr || actionQueue.size() == 0 && currentAction == nullptr)
		{

			planner = new Planner();

			// Goals are already sorted from most important to least important.
			for (auto& sg : goals)
			{
				actionQueue = planner->plan(availableActions, sg.second->goals, agentBeliefs);

				if (actionQueue.size() > 0)
				{
					currentGoal = sg.second;
					return; // We got a new Goal. So just return and wait for reiteration.
				}
			}
		}


		// Check whether we ran out of things to do and need a new plan.
		// This implies that our previous goal was achieved.
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

	}


	void completeAction()
	{
		using namespace std;

		cout << color(colors::GREEN);
		cout << "Action \""<< currentAction->action_name << "\" completed" << endl;

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

	double walking_speed = 0.1;
	
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
		double time = GameWorldTime::get()->getDaytime();

		for (auto& task : daySchedule->schedule)
		{

			double start = task->start;
			double end = task->end;

			// Is it time to pursue the roal goal?
			if (start < time && end > time)
			{
				// Check whether the goal is already in goal map before adding it again.
				for (auto& g : goals)
				{
					for (auto& sg : g.second->goals)
					{
						for (auto& aa : availableActions)
						{
							if (aa->action_name.compare(task->name) == 0)
							{
								return false;
							}
						}
					}
				}


				// TESTING
				// If it is time for new goal, we just append it to the map.
				// We need to get the goal name of the task, in order for it to work
				Action* action = nullptr;
				for (auto& a : availableActions)
				{
					if (a->action_name.compare(task->name) == 0)
					{
						action = a;
					}
				}

				int xxx = 1;
				for (auto& e : action->effects)
				{
					SubGoal* new_goal = new SubGoal(e->key, e->value, true);
					goals.emplace(std::make_pair(xxx++, new_goal)); // Add a new Goal.
				}


				/*
				// Before changing, check whether we are currently executing the goal.
				if (currentGoal)
				{

					std::string curr_goal_name = currentGoal->getName();
					if (task->name.compare(curr_goal_name) == 0)
					{
						return false;
					}
				}

				// Time to change goal.
				// For now we do this apruptly.

				if (currentAction != nullptr && currentAction->running)
				{
					if (!_wasCurrentGoalAchieved())
					{
						return false; // Try further to achieve goal.
					}
					else
					{
						completeAction(); // Else complete the current action as achieved, and get new goal from schedule.
					}
				}

				goals.clear(); // Clear all previous goals.


				// We need to get the goal name of the task, in order for it to work
				Action* action = nullptr;
				for (auto& a : availableActions)
				{
					if (a->action_name.compare(task->name) == 0)
					{
						action = a;
					}
				}

				if (!action) return false;

				int xxx = 1;
				for (auto& e : action->effects)
				{
					SubGoal* new_goal = new SubGoal(e->key, e->value, true);
					goals.emplace(std::make_pair(xxx++, new_goal)); // Add a new Goal.
				}
				*/
				
				return true;
			}

		}

		return false;
	}



	bool _initAction(Action* a);

	bool _wasCurrentGoalAchieved()
	{
		for (auto& g : currentGoal->goals)
		{
			if (!agentBeliefs->getStates().contains(g.second))
			{
				return false;
			}
		}

		return true;
	}

};