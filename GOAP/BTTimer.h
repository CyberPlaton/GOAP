#pragma once

#include "BTCondition.h"

class BTTimer : public BTCondition
{
public:
	enum class Policy
	{
		Invalid = -1,
		Smaller = 0,
		Greater = 1
	};

	enum class Granularity
	{
		Invalid = -1,
		Seconds = 0,
		Milliseconds = 1,
		Microseconds = 2
	};
public:

	/*
	* The first timer test will be unsuccessful, as creation time and first tick time are
	* probably with a huge difference.
	*
	* Thus the working will normalize with first several tree ticks.
	*/
	BTTimer(std::string name, Granularity g, Policy p, double condition) : BTCondition(name), m_Policy(p), m_Condition(condition), m_Granularity(g) { m_Timer.startTimer(); }
	BTTimer(std::string name, int granularity, int policy, double condition) : BTCondition(name), m_Policy(BTTimer::Policy(policy)), m_Condition(condition), m_Granularity(BTTimer::Granularity(granularity)) { m_Timer.startTimer(); }

	~BTTimer()
	{
		m_Condition = 0;
		m_Policy = (Policy)0;
		m_Granularity = (Granularity)0;
	}

	void exportToXML(tinyxml2::XMLElement* elem) final
	{
		std::string granularity = "none";
		std::string policy = "none";
		double cond = m_Condition;

		switch (m_Granularity)
		{
		case Granularity::Microseconds:
			granularity = "microseconds";
			break;

		case Granularity::Seconds:
			granularity = "seconds";
			break;

		case Granularity::Milliseconds:
			granularity = "milliseconds";
			break;
		}

		switch (m_Policy)
		{
		case Policy::Greater:
			policy = "greater";
			break;

		case Policy::Smaller:
			policy = "smaller";
			break;
		}

		elem->SetAttribute("granularity", granularity.c_str());
		elem->SetAttribute("policy", policy.c_str());
		elem->SetAttribute("condition", cond);
	}

	BTNodeResult checkCondition() override final
	{
		using namespace std;

		if (m_Policy == Policy::Invalid) return BTNodeResult::INVALID;


		double dt = m_Timer.getElapsedSeconds();
		double dtms = m_Timer.getElapsedMilliseconds();
		double dtus = m_Timer.getElapsedMicroseconds();


		cout << name() << " elapsedTime = " << dt << "s,    " << dtms << "ms,    " << dtus << "us" << endl;


		// Do timer checks based on the required granularity of time.
		switch (m_Granularity)
		{
		case Granularity::Seconds:
			if (m_Policy == Policy::Smaller)
			{

				if (dt <= m_Condition)
				{
					m_Timer.startTimer();
					return BTNodeResult::SUCCESS;
				}
			}
			else
			{
				if (dt >= m_Condition)
				{
					m_Timer.startTimer();
					return BTNodeResult::SUCCESS;
				}
			}
			break;





		case Granularity::Milliseconds:
			if (m_Policy == Policy::Smaller)
			{

				if (dtms <= m_Condition)
				{
					m_Timer.startTimer();
					return BTNodeResult::SUCCESS;
				}
			}
			else
			{
				if (dtms >= m_Condition)
				{
					m_Timer.startTimer();
					return BTNodeResult::SUCCESS;
				}
			}
			break;




		case Granularity::Microseconds:
			if (m_Policy == Policy::Smaller)
			{

				if (dtus <= m_Condition)
				{
					m_Timer.startTimer();
					return BTNodeResult::SUCCESS;
				}
			}
			else
			{
				if (dtus >= m_Condition)
				{
					m_Timer.startTimer();
					return BTNodeResult::SUCCESS;
				}
			}
			break;
		}



		if (m_Policy == Policy::Smaller)
		{
			m_Timer.startTimer();
		}

		return BTNodeResult::FAILURE;
	}

	std::string type() override
	{
		return "BTTimer";
	}

private:

	HRTimer m_Timer;
	Policy m_Policy = Policy::Invalid;
	Granularity m_Granularity = Granularity::Invalid;
	double m_Condition = 0.0;
};