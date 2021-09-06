#pragma once

#include "BTNode.h"

class BTCondition : public BTNode
{
public:

	BTCondition(std::string name) : m_Name(name)
	{

	}

	virtual ~BTCondition()
	{
	}

	void exportToXML(tinyxml2::XMLElement*) override
	{
		// Nothing to export.
	}

	bool hasBlackboard() override
	{
		return false;
	}

	void setBlackboard(BTBlackboard*) override
	{
		// Nothing to do.
	}

	BTBlackboard* getBlackboard() override
	{
		return nullptr;
	}

	virtual BTNodeResult checkCondition()
	{
		return INVALID;
	}

	BTNodeResult tick() override
	{
		if (checkCondition() == BTNodeResult::SUCCESS)
		{
			return SUCCESS;
		}
		else
		{
			return FAILURE;
		}
	}


	BTNode* parent() override
	{
		return m_Parent;
	}


	void setParent(BTNode* node) override
	{
		m_Parent = node;
	}


	BTNode* child(std::string) override
	{
		return nullptr;
	}


	std::map<int, BTNode*>  children() override
	{
		std::map<int, BTNode*> v;
		return v;
	}


	void addChild(BTNode*) override
	{
		return;
	}


	void removeChild(std::string) override
	{
		return;
	}

	void removeFirstChild() override
	{
		return;
	}

	std::string name() override
	{
		return m_Name;
	}

	std::string type() override
	{
		return "BTCondition";
	}

	void freeMemory() override
	{
		using namespace std;
		cout << "Deleting Node \"" << m_Name << "\"" << endl;

		m_Name.clear();
		m_Parent = 0;
		delete this;
	}

private:


	BTNode* m_Parent = nullptr;

	std::string m_Name;
};