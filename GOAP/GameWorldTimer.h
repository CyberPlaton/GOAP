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
		now = _currentTime();
	}



	/*
	* Returns the elapsed gamehours since start, where 2.4 means 2 Hours and 40 Minutes
	* elapsed since start.
	* Note that first digit can only go to 12 and second digit only to 6,
	* anything else is invalid...
	*/
	double getElapsedGamehours(bool print = false)
	{
		using namespace std;

		if (print)
		{
			cout << color(colors::MAGENTA);
			cout << "Elapsedgamehours: " << white;

			double d = _currentTime();

			cout << color(colors::RED);
			cout << "CurrTime: " << d << " - " << " TimerStart: " << now << "  =>  Duration: " << d - now << endl;
			cout << white;
		}

		return _currentTime() - now;
	}


private:
	double now = 0.0;

	double _currentTime()
	{
		return std::stod(GameWorldTime::get()->getDaytimeString());
	}
};