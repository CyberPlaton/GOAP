#pragma once

#include <string>
#include <vector>
#include <map>
#include <array>


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


	/*
	* General Node information.
	*/
	virtual std::string name() = 0;
};