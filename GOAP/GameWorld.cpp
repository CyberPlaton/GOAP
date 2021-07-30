#include "GameWorld.h"

GameWorld* GameWorld::g_GameWorld = nullptr;


GameWorld* GameWorld::get()
{
	if (!g_GameWorld)
	{
		g_GameWorld = new GameWorld();

		g_GameWorld->world = new WorldStates();
	}

	return g_GameWorld;
}

void GameWorld::del()
{
	if (g_GameWorld)
	{
		delete g_GameWorld;
	}
}