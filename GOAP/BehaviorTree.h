#pragma once

#include "BTNodes.h"
#include "BTBlackboard.h"
#include "Any.h"

class BTFactory;

class BehaviorTree
{
	friend class BTFactory;

public:
	BehaviorTree(std::string name) : m_Name(name) {}
	~BehaviorTree()
	{
		/*
		* Remove the tree structure.
		* Where the behavior tree is responsible for freeing memory used by its nodes.
		*/
		while (m_TreeNodes.size() > 0)
		{
			BTNode* node = m_TreeNodes[m_TreeNodes.size() - 1];
			node->freeMemory();
			m_TreeNodes.pop_back();
		}

		m_Root = 0;

		m_Name.clear();
		m_TreeNodes.clear();
	}

	BTNodeResult update()
	{
		return m_Root->tick();
	}

	void setRoot(BTNode* node)
	{
		m_Root = node;
	}

	BTNode* getRoot()
	{
		return m_Root;
	}


	BTNode* findNode(const std::string& name)
	{
		for (auto& n : m_TreeNodes)
		{
			if (n->name().compare(name) == 0) return n;
		}

		return nullptr;
	}



private:

	std::vector<BTNode*> m_TreeNodes;

	BTNode* m_Root = nullptr;

	std::string m_Name;

};
