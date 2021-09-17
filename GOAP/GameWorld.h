#pragma once

#include "WorldState.h"

class GameWorld
{
public:

	static GameWorld* get();

	static void del();

	WorldStateCmp* getWorldState()
	{
		return world;
	}

private:

	WorldStateCmp* world = nullptr;

	GameWorld() {}

	static GameWorld* g_GameWorld;
};