#pragma once

#include <vector>
#include <string>
#include "Action.h"
#include "ActionDatabase.h"

struct Action;

struct ScheduleEntry;
class DayScheduleDefinition;
class DaySchedule;


using ScheduleID = std::string;
using ScheduleEntryID = std::string;
using ScheduleEntries = std::vector< ScheduleEntry* >;
using ActionEntries = std::vector< Action* >;
using ActionWeightedEntries = std::map< Action*, unsigned int >;

/*
* A DayScheduleDefinition is kind of a prefab from which
* a DaySchedule can be constructed.
*/
class DayScheduleDefinition
{
public:
	DayScheduleDefinition(const ScheduleID& id) 
		: id(id)
	{

	}


	void addEntry(ScheduleEntry* entry)
	{
		entries.push_back(entry);
	}



private:
	ScheduleID id = "Invalid";
	ScheduleEntries entries;
};


/*
* A ScheduleEntry is an entry in the daily schedule of an agent
* defining the action it should take executing the entry.
*/
struct ScheduleEntry
{
	ScheduleEntry(const ScheduleEntryID& name, double min, double max)
	{
		this->id = name;
		this->minDuration = min;
		this->maxDuration = max;
	}


	/*
	* Interface functions.
	*/

	/*
	* Add a new action to schedule.
	*/
	virtual void addAction(Action*, int) = 0;

	/*
	* Let the ScheduleEntry choose the next action for execution.
	*/
	virtual Action* chooseNewAction(Action*) = 0;

	/*
	* Clone this ScheduleEntry and return a reference to the new instance.
	*/
	virtual ScheduleEntry* clone(ScheduleEntry*) = 0;




	ScheduleEntryID id;
	double minDuration;
	double maxDuration;
};




/*
* An instance of a daily schedule which is meant to be attached
* to an agent.
*/
class DaySchedule
{
public:
	DaySchedule(const ScheduleID& id) : id(id){}


	ScheduleEntries schedule;
	ScheduleID id;
};



/*
* An entry with one basic action.
*/
struct BasicScheduleEntry : public ScheduleEntry
{
	BasicScheduleEntry(const ScheduleID& id, double min, double max) : ScheduleEntry(id, min, max)
	{

	}


	void addAction(Action*a, int) override final
	{
		if (action) delete action; action = 0;
		action = a;
	}

	Action* chooseNewAction(Action* last_action) override final
	{
		action = last_action;
		return action;
	}

	ScheduleEntry* clone(ScheduleEntry* e) override final
	{
		BasicScheduleEntry* entry = new BasicScheduleEntry(e->id, e->minDuration, e->maxDuration);
		return entry;
	}

	Action* action = nullptr;
};


/*
* An entry with several action to choose from.
* Where the entry chooses randomly based on a weight function an action
* to be executed.
*/
struct WeightedRandomScheduleEntry : public ScheduleEntry
{
	WeightedRandomScheduleEntry(const ScheduleID& id, double min, double max) : ScheduleEntry(id, min, max)
	{

	}

	
	void addAction(Action* a, int weight) override final
	{
		actions.emplace(a, weight);
	}

	Action* chooseNewAction(Action* last_action) override final
	{
		// Not implemented
		return nullptr;
	}

	ScheduleEntry* clone(ScheduleEntry* e) override final
	{
		auto toClone = static_cast<WeightedRandomScheduleEntry*>(e);

		WeightedRandomScheduleEntry* entry = new WeightedRandomScheduleEntry(e->id, e->minDuration, e->maxDuration);
		for (auto& a : toClone->actions)
		{
			entry->actions[a.first] = a.second;
		}

		return entry;
	}

	ActionWeightedEntries actions;
};



/*
* An entry with several actions, which are executed one after another.
*/
struct SequencedScheduleEntry : public ScheduleEntry
{
	SequencedScheduleEntry(const ScheduleID& id, double min, double max) : ScheduleEntry(id, min, max)
	{

	}


	void addAction(Action* a, int) override final
	{
		actions.push_back(a);
	}

	Action* chooseNewAction(Action* last_action) override final
	{
		// Not implemented.
		return nullptr;
	}

	ScheduleEntry* clone(ScheduleEntry* e) override final
	{
		auto toClone = static_cast<SequencedScheduleEntry*>(e);

		SequencedScheduleEntry* entry = new SequencedScheduleEntry(e->id, e->minDuration, e->maxDuration);
		for (auto& e : toClone->actions)
		{
			entry->actions.push_back(e);
		}

		return entry;
	}

	ActionEntries actions;
};