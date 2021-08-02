#pragma once

#include<vector>
#include <string>


struct ScheduleEntry
{
	ScheduleEntry(const std::string& name, double start, double end)
	{
		this->name = name;
		this->start = start;
		this->end = end;
	}


	std::string name;
	double start;
	double end;
};

class DaySchedule
{
public:
	DaySchedule(const std::string& name):name(name){}


	void addEntry(const std::string& name, double start, double end)
	{
		schedule.push_back(new ScheduleEntry(name, start, end));
	}

	void addEntry(ScheduleEntry* e)
	{
		schedule.push_back(e);
	}



	void removeEntry(const std::string& name)
	{
		for (int i = 0; i < schedule.size(); i++)
		{
			if (schedule[i]->name.compare(name) == 0)
			{
				schedule.erase(schedule.begin() + i);
			}
		}
	}


	std::vector< ScheduleEntry* > schedule;
	std::string name;
};