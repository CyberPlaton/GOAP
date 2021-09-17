#pragma once

#include "BTDecorator.h"


class BTInverter : public BTDecorator
{
public:

	BTInverter(std::string name) : BTDecorator(name)
	{

	}

	virtual ~BTInverter()
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

	void setBlackboard(BTBlackboard* bt) override
	{
		// Nothing to do.
	}

	BTBlackboard* getBlackboard() override
	{
		return nullptr;
	}

	BTNodeResult tick() override final
	{
		BTNodeResult result = m_Child->tick();

		if (result == BTNodeResult::SUCCESS)
		{
			return FAILURE;
		}
		else
		{
			return SUCCESS;
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
		return m_Child;
	}


	std::map<int, BTNode*>  children() override
	{
		std::map<int, BTNode*> v;
		if (m_Child)
		{
			v.emplace(0, m_Child);
		}

		return v;
	}


	void addChild(BTNode* node) override
	{
		if (!m_Child)
		{
			m_Child = node;

			node->setParent(this);
		}
	}


	void removeChild(std::string) override
	{
		if (m_Child)
		{
			delete m_Child;
			m_Child = nullptr;
		}
	}

	void removeFirstChild() override
	{
		if (m_Child)
		{
			delete m_Child;
			m_Child = 0;
		}
	}


	std::string name() override
	{
		return m_Name;
	}


	std::string type() override
	{
		return "BTInverter";
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
	BTNode* m_Child = nullptr;

	std::string m_Name;
};