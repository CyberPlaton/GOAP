#include "BehaviorTreeDefinitions.h"

BehaviorTree* createShopkeeperBehaviorTree(const std::string& name, GameObject* pawn)
{
	BehaviorTree* tree = nullptr;
	BTFactory factory(name);

	tree = factory.add<BTFallback>("InitialNode")
					.add<BTMoveToRandomPosition>("RandomMovement", pawn)
					.end()
		.build();

	return tree;
}