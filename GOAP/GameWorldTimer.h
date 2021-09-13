#pragma once

#include "GameWorldTime.h"
#include "ColorConsole.h"


/*
* A timer for ingame time.
* Good for measuring elapsed hours and minutes in game time and not realworld seconds etc.
*/
class GameworldTimer
{
public:
	GameworldTimer(){}
	~GameworldTimer(){}

	/*
	* Start timer counter from now.
	*/
	void startTimer()
	{
		counter_stop = _currentTime();
		startday = GameWorldTime::get()->getDay();
		startweek = GameWorldTime::get()->getWeek();
		startmonth = GameWorldTime::get()->getMonth();
	}


	double currentTime() { return _currentTime(); }

	/*
	* Returns the elapsed gamehours since start, where 2.4 means 2 Hours and 40 Minutes
	* elapsed since start.
	* Note that first digit can only go to 12 and second digit only to 6,
	* anything else is invalid...
	*/
	double getElapsedGamehours()
	{
		using namespace std;

		double starttime = counter_stop;
		double endtime = _currentTime();

		// End of elapsed time on next day
		if (!_isEndOnSameDay())
		{
			// How long until end of day
			double a = 24.0 - starttime;

			// Return elapsed time
			return a + endtime;
		}
		// End of elapsed time on same day
		else
		{
			return endtime - starttime;
		}
	}


private:
	double counter_stop = 0.0;
	
	int startday = 0;
	int startweek = 0;
	int startmonth = 0;

	double _currentTime()
	{
		return std::stod(GameWorldTime::get()->getDaytimeString());
	}


	bool _isEndOnSameDay()
	{
		if (startday != GameWorldTime::get()->getDay() || 
			startweek != GameWorldTime::get()->getWeek() || 
			startmonth != GameWorldTime::get()->getMonth())
		{
			return false;
		}

		return true;
	}
};