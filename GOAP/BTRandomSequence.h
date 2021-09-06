#pragma once

#include "BTNode.h"


#include <random>
#include <time.h>
class BTRandomSequence : public BTNode
{
public:
	BTRandomSequence(std::string name) : m_Name(name)
	{

	}

	virtual ~BTRandomSequence()
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


	BTNodeResult tick() override
	{
		// Randomize the children execution sequence.
		std::vector<BTNode*> shuffled_kids;
		for (auto& it : m_Children)
		{
			shuffled_kids.push_back(it.second);
		}


		auto rng = std::default_random_engine{};
		rng.seed(time(NULL));
		std::shuffle(std::begin(shuffled_kids), std::end(shuffled_kids), rng);


		for (auto& kid : shuffled_kids)
		{
			BTNodeResult result = kid->tick();

			if (result == BTNodeResult::RUNNING)
			{
				return BTNodeResult::RUNNING;
			}
			else if (result == BTNodeResult::FAILURE || result == BTNodeResult::INVALID)
			{
				return BTNodeResult::FAILURE;
			}
		}

		return BTNodeResult::SUCCESS;
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
		return "BTRandomSequence";
	}

	void freeMemory() override
	{
		using namespace std;
		cout << "Deleting Node \"" << m_Name << "\"" << endl;

		m_Children.clear();
		m_Name.clear();
		m_Parent = 0;
		delete this;
	}

private:


	BTNode* m_Parent = nullptr;

	/*
	* The first number indicates the positional value of the node,
	* where 0 means this node is executed first and n-1 is the last node.
	*/
	std::map<int, BTNode*> m_Children;
	int m_NextNodeIndex = 0;

	std::string m_Name;

};