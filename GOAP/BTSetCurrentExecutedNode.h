#pragma once

#include "ColorConsole.h"

#include "BTBlackboard.h"
#include "BTDecorator.h"
#include "BehaviorTree.h"


class BTSetCurrentExecutedNode : public BTDecorator
{
public:
	BTSetCurrentExecutedNode(const std::string& name, BTBlackboard* blackboard) :
		BTDecorator(name), blackboard(blackboard)
	{
	}


	BTNodeResult tick() override final
	{
		using namespace std;

		if (this->child(""))
		{
			// This node can only have one child, which is executed after it,
			// which we set as current executed.
			BTNode* current_executed = this->child("");


			blackboard->set<BTNode*>("CurrentExecutedNode", current_executed, "BTNode");

			
			cout << color(colors::GREEN);
			cout << "[BTSetCurrentExecutedNode] Setting current executed node to \""<< current_executed->name() << "\"" << white << endl;

			return BTNodeResult::SUCCESS;
		}


		cout << color(colors::RED);
		cout << "[BTSetCurrentExecutedNode] Setting current executed node to \"nullptr\"" << white << endl;

		blackboard->set<BTNode*>("CurrentExecutedNode", nullptr, "BTNode");
		return BTNodeResult::FAILURE;
	}


private:
	BTBlackboard* blackboard = nullptr;
	BehaviorTree* tree = nullptr;
};