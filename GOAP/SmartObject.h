#pragma once

#include "ComponentSystem.h"

#include "tinyxml2.h"


/*
* See "Smart Objects".
*/
class SmartObject : public Component
{
public:
	SmartObject(const ComponentID& name)
	{
		this->name = name;
		type = "SmartObject";
		init(type);
	}

	ComponentType getType() override { return this->type; }


	/*
	* Import Definition from XML.
	*/
	bool loadDefinition(const std::string& path);


	/*
	* Returns the associated action for a need.
	* The returned string is the ActionID, thus it should match with what is defined for the action itself 
	* and the smart object.
	*/
	std::string getAssociatedAction(const std::string& need)
	{
		auto a = actionsMap.find(need);
		if (a != actionsMap.end())
		{
			return a->second;
		}

		return "";
	}


	/*
	* Returns the amount of fulfillment or 0.0 if does not fulfill.
	*/
	double getFulfillmentAmount(const std::string& name)
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
	ComponentType type;

	/*
	* Map of how much this object fulfills a certain need.
	*/
	std::map<std::string, double> needsFulfillmentMap;

	/*
	* Map of associated actions with a need.
	* E.g. { "sleep" : "ActionSleep" }
	*/
	std::map< std::string, std::string > actionsMap;
};