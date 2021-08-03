#include "GameWorldTime.h"

GameWorldTime* GameWorldTime::g_GameWorldTime = nullptr;


GameWorldTime* GameWorldTime::get()
{
	if (!g_GameWorldTime)
	{
		g_GameWorldTime = new GameWorldTime();
	}

	return g_GameWorldTime;
}

void GameWorldTime::del()
{
	if (g_GameWorldTime)
	{
		delete g_GameWorldTime;
	}
}