#pragma once
#include "tinyxml2.h"

#include "GameObject.h"

#include <string>
#include <stdexcept>


class ActionMoveToDestination;
class ActionSleep;
class ActionEat;

using ActionID = std::string;

using ScoringFunction = double(*)(double);


/*
* A ActionDefinition is basically a prefab from which actions can
* be constructed on the fly.
*/
struct ActionDefinition
{
	ActionDefinition(const std::string& filepath)
	{
		using namespace tinyxml2;

		tinyxml2::XMLDocument doc;
		XMLError result = doc.LoadFile(filepath.c_str());
		if (result != XMLError::XML_SUCCESS)
		{
			throw std::runtime_error("Could not load action definition!");
		}


		XMLNode* root = doc.FirstChild();
		if (root == nullptr) throw std::runtime_error("Could not load action definition!");


		
		// Get id
		XMLElement* elem = root->FirstChildElement("ID");
		this->id = elem->GetText();

		// Set hash
		this->hash = hasher(this->id);


		// Get duration
		elem = root->FirstChildElement("MinDuration");
		this->minDuration = std::stod(elem->GetText());

		elem = root->FirstChildElement("MaxDuration");
		this->maxDuration = std::stod(elem->GetText());


		// Get tag of the target
		elem = root->FirstChildElement("TargetTag");
		this->targetTag = elem->GetText();
	}



	double minDuration = 0.0;
	double maxDuration = 0.0;

	size_t hash;
	ActionID id;
	std::string targetTag;
};



class Action
{
public:

	/*
	* Get the associated gameobject for this action.
	*/
	virtual GameObject* getPawn() = 0;

	/*
	*/
	virtual GameObject* getSmartObject() = 0;


	/*
	* Get the id of the action.
	*/
	virtual ActionID getID() = 0;


	virtual std::string getTargetTag() = 0;


	/*
	* Get the duration of the action.
	*/
	virtual double getMinDuration() = 0;
	virtual double getMaxDuration() = 0;
	virtual double getDuration() = 0;


	virtual double getStartTime() = 0;
	virtual double getEndTime() = 0;
	virtual double getLeftTime() = 0;

	/*
	* Perform the action. Return true if the action was successfully completed.
	*/
	virtual bool perform(double dt) = 0;


	/*
	* Special function for what happens if Action
	* finished executing its perform function.
	*/
	virtual bool postPerform(double dt)  = 0;
};


class ActionInstance : public Action
{
public:
	ActionInstance(ActionDefinition* def, GameObject* pawn, GameObject* smartObject) : pawn(pawn), smartObject(smartObject)
	{
		this->id = def->id;
		this->minDuration = def->minDuration;
		this->maxDuration = def->maxDuration;
		this->targetTag = def->targetTag;
	}

	GameObject* getPawn() override final
	{
		return this->pawn;
	}

	GameObject* getSmartObject() override final
	{
		return this->smartObject;
	}

	ActionID getID() override final
	{
		return this->id;
	}

	double getMinDuration() override final
	{
		return this->minDuration;
	}

	double getMaxDuration() override final
	{
		return this->maxDuration;
	}


	double getDuration() override final
	{
		return getMaxDuration() - getMinDuration();
	}

	std::string getTargetTag() override final
	{
		return this->targetTag;
	}

	virtual double getStartTime() { return (double)INT_MAX; }
	virtual double getEndTime() { return (double)INT_MAX; }
	virtual double getLeftTime() { return (double)INT_MAX; }

	
	virtual bool perform(double dt)
	{
		return true;
	}

	virtual bool postPerform(double dt)
	{
		return true;
	}
	


private:
	ActionID id;
	double minDuration = 0.0;
	double maxDuration = 0.0;
	GameObject* pawn = nullptr;
	std::string targetTag;
	GameObject* smartObject = nullptr;
};