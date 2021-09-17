#pragma once

#include "BTCommon.h"

class BTBlackboard;
class BehaviorTree;

enum BTNodeResult
{
	INVALID = -1,
	FAILURE = 0,
	SUCCESS = 1,
	RUNNING = 2
};

class BTNode
{
public:

	/*
	* Base function for execution on each update tick.
	*/
	virtual BTNodeResult tick() = 0;


	/*
	* Behavior Tree Graph related functions.
	*/
	virtual BTNode* parent() = 0;
	virtual void setParent(BTNode*) = 0;
	virtual BTNode* child(std::string) = 0;
	virtual std::map<int, BTNode*>  children() = 0;
	virtual void addChild(BTNode*) = 0;
	virtual void removeChild(std::string) = 0;
	virtual void removeFirstChild() = 0;
	virtual void freeMemory() = 0;

	virtual bool hasBlackboard() = 0;
	virtual void setBlackboard(BTBlackboard*) = 0;
	virtual BTBlackboard* getBlackboard() = 0;

	/*
	* General Node information.
	*/
	virtual std::string name() = 0;
	virtual std::string type() = 0;


	/*
	* Export/Import to XML
	*/
	virtual void exportToXML(tinyxml2::XMLElement*) = 0;
};