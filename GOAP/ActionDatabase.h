#pragma once

#include "Action.h"
#include "Actions.h"

#include "tinyxml2.h"


/*
* The database holds the "prefabs" for the actions.
*/
class ActionDatabase
{
public:

	static ActionDatabase* get()
	{
		if (!g_ActionDatabase)
		{
			g_ActionDatabase = new ActionDatabase();
		}

		return g_ActionDatabase;
	}


	static void del()
	{
		if (g_ActionDatabase)
		{
			g_ActionDatabase->actionPrefabs.clear();
			delete g_ActionDatabase;
		}
	}


	bool loadDefinitions(const std::string& filepath);


	ActionDefinition* getActionDefinition(ActionID id)
	{
		for (auto& a : actionPrefabs)
		{
			if (a.second->hash == hasher(id)) return a.second;
		}

		return nullptr;
	}


	template < typename ActionType, class... Args >
	ActionType* constructAction(ActionID id, GameObject* pawn, GameObject* smartObject, Args... args)
	{
		using namespace std;

		ActionDefinition* def = nullptr;
		def = getActionDefinition(id);

		if (!def) return nullptr;

		cout << "Constructing Action \""<< def->id << "\"" << endl;
		cout << "Target: " << def->targetTag << endl;
		cout << "MinDur: " << def->minDuration << endl;
		cout << "MaxDur: " << def->minDuration << endl;


		ActionType* action = new ActionType(def, pawn, smartObject, args...);
		if (action) return action;


		cout << "Could not construct Action \"" << id << "\"!" << endl;

		return nullptr;
	}

private:

	ActionDatabase(){}

	static ActionDatabase* g_ActionDatabase;

	std::map< ActionID, ActionDefinition* > actionPrefabs;
};