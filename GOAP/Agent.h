#pragma once

#include "ColorConsole.h"

#include "ComponentSystem.h"
#include "Components.h"
#include "GameObject.h"
#include "SmartObject.h"
#include "OwnershipCmp.h"

#include "Inventory.h"
#include "WorldState.h"
#include "DaySchedule.h"

#include "AgentNeeds.h"
#include "AgentRole.h"
#include "AgentStats.h"

#include "Action.h"


#define EULER_NUMBER 2.71828


/*
* An agent is an extended gameobject with game specific functionality.
* 
* It has following components by default:
* Renderable,
* Transform,
* CollisionBox,
* Stats,
* Needs,
* Role,
* Navigator
*/
class Agent : public GameObject
{
public:
	Agent(const std::string& tag, const std::string& name) : GameObject(tag, name)
	{
		agentInventory = new Inventory();
		AddComponent(new AgentStatsCmp(name + "_Stats"));
		AddComponent(new AgentNeedsCmp(name + "_Needs"));
		//AddComponent(new AgentRoleCmp("Role", "None"));
		AddComponent(new NavigatorCmp("Navigator", this));
		AddComponent(new RendererableCmp("Renderable", 1.0f, 1.0f, "red"));
		AddComponent(new TransformCmp("Transform"));
		AddComponent(new AnimatorCmp("Animator", this));

		agentBeliefs = new WorldStates();
		timer = new GameworldTimer();
	}

	void update(double dt) override final;





public:
	static double AGENT_HUNGER_SCORE;
	static double AGENT_HUNGER_SCORE_STEEPNESS;
	static double AGENT_SLEEP_SCORE;
	static double AGENT_SLEEP_SCORE_STEEPNESS;

	bool isIdling = false;

public:

	std::map< std::string, double > needsScoreMap;
	std::stack< Action* > actionStack;
	Inventory* agentInventory = nullptr;
	std::vector<GameObject*> agentOwnedObjects;

	WorldStates* agentBeliefs = nullptr;

	GameworldTimer* timer = nullptr;

private:
	/*
	* Use scoring functions to score the needs in the map.
	*/
	void scoreNeeds();

	/*
	* Execute a scoring function.
	*/
	double score(ScoringFunction f, GameObject* pawn, double input)
	{
		return f(pawn, input);
	}


	/*
	* Use a function to get the need we want currently to battle.
	*/
	std::string getMostValuableGoal();

	/*
	* Search for object which does satisfy the need.
	* Searching is done in owned object or inventory and then in the world around.
	*/
	GameObject* getObjectToFulfillNeedWith(const std::string& need);

	/*
	* Get the action of a smartobject associated with given need.
	* If the SmartObject does not satisfy the need given, it returns nullptr.
	*/
	Action* getActionOfSmartObjectForNeed(GameObject* go, const std::string& need);
};


/*
* Hunger Scoring function.
* Behaves somewhat exponentially.
* 
* Description: Score is rising very lowly in the beginning, for 1-2 Gamedays,
* when it reaches a considerable level of +30-40 then it starts to grow somewhat fast.
*/
double scoreAgentHunger(GameObject* pawn, double hunger);


/*
*/
double scoreAgentSleep(GameObject* pawn, double sleep);