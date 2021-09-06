#pragma once

#include "BTNodes.h"
#include "BehaviorTree.h"


class BTFactory
{
public:
	BTFactory(const std::string& tree_name) : m_Tree(new BehaviorTree(tree_name)) {}

	~BTFactory()
	{
		last_parent = 0;
		m_Tree = 0;
		has_root = false;
	}


	static bool exportBehaviorTree(BehaviorTree* tree);


	/*
	* Add a node as child to the current worked on node.
	* The Arguments must match the required arguments for the constructor.
	*/
	template < typename NodeType, class... Args >
	BTFactory& add(const std::string& node_name, Args... args)
	{
		using namespace std;

		NodeType* node = new NodeType(node_name, args...);

		if (!has_root)
		{
			cout << "Create Root Node: " << node->name() << endl;

			m_Tree->setRoot(node);
			has_root = true;
			last_parent = node;
		}
		else
		{
			cout << "Create Node: " << node->name();
			cout << " with Parent: " << last_parent->name() << endl;

			last_parent->addChild(node);

			last_parent = node;
		}

		m_Tree->m_TreeNodes.push_back(node);

		return *this;
	}


	/*
	* End Adding children to current Node and fallback to its parent.
	*/
	BTFactory& end()
	{
		using namespace std;
		cout << "Getting Last Parent: ";

		BTNode* p = last_parent->parent();
		if (p != nullptr)
		{
			cout << p->name() << endl;
			last_parent = p;
		}
		else
		{
			cout << " none" << endl;
		}

		return *this;
	}


	/*
	* Return the complete builded tree.
	*/
	BehaviorTree* build()
	{
		return m_Tree;
	}


private:

	BTNode* last_parent = nullptr;
	BehaviorTree* m_Tree = nullptr;

	bool has_root = false;


private:

};