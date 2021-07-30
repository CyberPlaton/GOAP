#pragma once

#include "WorldState.h"

class GameWorld
{
public:

	static GameWorld* get();

	static void del();

	WorldStates* getWorldState()
	{
		return world;
	}

private:

	WorldStates* world = nullptr;

	GameWorld() {}

	static GameWorld* g_GameWorld;
};