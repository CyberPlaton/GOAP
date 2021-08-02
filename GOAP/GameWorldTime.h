#pragma once

#include "Timer.h"

class GameWorldTime
{
public:

	static GameWorldTime* get();

	static void del();

	void setTimeSpeed(double s)
	{
		this->timeSpeed = s;
	}

	double getTimeSpeed()
	{
		return this->timeSpeed;
	}

	void update()
	{
		// Let daytime pass a bit.
		daytime += timeSpeed;


		if (daytime >= 24.00)
		{
			// Day elapsed.
			day++;

			daytime = 0.00;
		}

		// Adjust counters.
		if (day > 7)
		{
			// Week elapsed
			day = 1;

			week++;
		}


		if (week > 4)
		{
			// Month elapsed.
			week = 1;

			month++;
		}


		if (month > 12)
		{
			// Year elapsed.
			month = 1;

			year++;
		}
	}


	double getDaytime()
	{
		return daytime;
	}

	int getDay()
	{
		return day;
	}


	int getWeek()
	{
		return week;
	}


	int getMonth()
	{
		return month;
	}


	int getYear()
	{
		return year;
	}


private:

	GameWorldTime() {}

	static GameWorldTime* g_GameWorldTime;


	double timeSpeed;

	double daytime = 0.0;

	int day = 1;
	int week = 1;
	int month = 1;
	int year = 1;
};




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