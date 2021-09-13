#pragma once

#include "Timer.h"
#include <tuple>

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
		internal_minute_counter += timeSpeed;

		if (internal_minute_counter >= 1.0)
		{
			internal_minute_counter = 0.0;
			minutes++;
		}


		if (minutes >= 60)
		{
			hours++;
			minutes = 0;
		}


		if (hours >= 24)
		{
			// Day elapsed.
			day++;

			hours = 0;
		}

		// From here on we  have here modulo from 1-8 as e.g. "day 0" does not exist.
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
		double time = stod(getDaytimeString());
		return time;
	}

	std::string getDaytimeString()
	{
		int hour = hours;
		int min = minutes;

		// We want the time format as follows: "14.04".
		std::string timestring = std::to_string(hour);

		if (min < 10)
		{
			timestring += ".0" + std::to_string(min);
		}
		else
		{
			timestring += "." + std::to_string(min);
		}

		return timestring;
	}

	int getMinutes()
	{
		return minutes;
	}

	int getHours()
	{
		return hours;
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

	double internal_minute_counter = 0.0;

	int minutes = 0;
	int hours = 0;

	int day = 1;
	int week = 1;
	int month = 1;
	int year = 1;
};