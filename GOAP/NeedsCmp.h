#pragma once

#include "ComponentSystem.h"
#include "tinyxml2.h"

class NeedsCmp : public Component
{
public:
	NeedsCmp()
	{
		this->name = name;
		type = "Needs";
		init(type);
	}

	std::string getType() override { return this->type; }


	/*
	* Import Definition from XML.
	*/
	bool init(const std::string& path)
	{
		// Not implemented
		return false;
	}


	/*
	* Perform a certain action and return the amount of fulfillment
	* from that action.
	*/
	double performAction(const std::string& name)
	{
		// Not implemented
		return 0.0;
	}


	/*
	* Returns whether a certain action can be performed on this object
	* to fulfill a need amount.
	* 
	* For example, a bed could have several possible action: like sleep, sit or have sex on it.
	*/
	bool doesAllowAction(const std::string& name)
	{
		// Not implemented
		return false;
	}




	/*
	* Returns the amount of fulfillment or 0.0 if does not fulfill.
	*/
	double doesFulfillNeed(const std::string& name)
	{
		for (auto& e : needsFulfillmentMap)
		{
			if (e.first.compare(name) == 0) return e.second;
		}

		return 0.0;
	}

	void setFulfillmentAmount(const std::string& name, double amount)
	{
		needsFulfillmentMap[name] = amount;
	}


private:
	std::string type;

	std::map<std::string, double> needsFulfillmentMap;
};