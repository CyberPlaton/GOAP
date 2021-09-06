#pragma once

#include "BTNode.h"


class BTParallel : public BTNode
{
public:

	enum Policy
	{
		Invalid = -1,
		Require_All = 0,
		Require_One = 1
	};


public:

	BTParallel(std::string name, Policy success, Policy fail) : m_Name(name), m_FailPolicy(fail), m_SuccessPolicy(success) {}
	BTParallel(std::string name, int success, int fail) : m_Name(name), m_FailPolicy(BTParallel::Policy(fail)), m_SuccessPolicy(BTParallel::Policy(success)) {}

	virtual ~BTParallel()
	{
	}

	void exportToXML(tinyxml2::XMLElement* elem) override final
	{
		std::string success = "none";
		std::string fail = "none";

		switch (m_SuccessPolicy)
		{
		case Policy::Require_All:
			success = "require_all";
			break;
		case Policy::Require_One:
			success = "require_one";
			break;
		}

		switch (m_FailPolicy)
		{
		case Policy::Require_All:
			fail = "require_all";
			break;
		case Policy::Require_One:
			fail = "require_one";
			break;
		}

		elem->SetAttribute("policy_success", success.c_str());
		elem->SetAttribute("policy_fail", fail.c_str());
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

	BTNodeResult tick() override
	{
		if (m_FailPolicy == Policy::Invalid) return INVALID;
		if (m_SuccessPolicy == Policy::Invalid) return INVALID;

		int successful_ticks = 0, failed_ticks = 0;


		for (auto& kid : m_Children)
		{

			BTNodeResult result = kid.second->tick();

			if (result == SUCCESS)
			{
				successful_ticks++;

				if (m_SuccessPolicy == Require_One)
				{
					return SUCCESS;
				}
			}
			else if (result == FAILURE)
			{
				failed_ticks++;

				if (m_FailPolicy == Require_One)
				{
					return SUCCESS;
				}
			}
			else
			{
				return INVALID;
			}
		}



		if (m_SuccessPolicy == Require_All &&
			successful_ticks == m_Children.size())
		{
			return SUCCESS;
		}

		if (m_FailPolicy == Require_All &&
			failed_ticks == m_Children.size())
		{
			return FAILURE;
		}


		// Should never reach.
		return INVALID;
	}


	BTNode* parent() override
	{
		return m_Parent;
	}

	void setParent(BTNode* node) override
	{
		m_Parent = node;
	}


	BTNode* child(std::string name) override
	{
		for (auto& kid : m_Children)
		{
			if (strcmp(name.c_str(), kid.second->name().c_str()) == 0)
			{
				return kid.second;
			}
		}

		return nullptr;
	}


	std::map<int, BTNode*>  children() override
	{
		return m_Children;
	}


	void addChild(BTNode* node) override
	{
		m_Children.try_emplace(m_NextNodeIndex, node);

		m_NextNodeIndex++;

		node->setParent(this);
	}


	void removeChild(std::string name) override
	{
		int location = 0;

		for (auto& kid : m_Children)
		{
			if (strcmp(kid.second->name().c_str(), name.c_str()) == 0)
			{
				m_Children.erase(location);
				m_NextNodeIndex--;
				return;
			}

			location++;
		}
	}

	void removeFirstChild() override
	{
		if (!m_Children.empty())
		{
			auto it = m_Children.begin();
			delete it->second;

			m_Children.erase(m_Children.begin());
		}
	}

	std::string name() override
	{
		return m_Name;
	}

	std::string type() override
	{
		return "BTParallel";
	}

	void freeMemory() override
	{
		using namespace std;
		cout << "Deleting Node \"" << m_Name << "\"" << endl;

		m_Children.clear();
		m_NextNodeIndex = 0;
		m_FailPolicy = (Policy)0;
		m_SuccessPolicy = (Policy)0;
		m_Parent = 0;
		m_Name.clear();
		delete this;
	}

private:

	Policy m_FailPolicy = Policy::Invalid;
	Policy m_SuccessPolicy = Policy::Invalid;


	BTNode* m_Parent = nullptr;

	/*
	* The first number indicates the positional value of the node,
	* where 0 means this node is executed first and n-1 is the last node.
	*/
	std::map<int, BTNode*> m_Children;
	int m_NextNodeIndex = 0;

	std::string m_Name;
};