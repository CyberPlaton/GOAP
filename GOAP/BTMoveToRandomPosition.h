#pragma once

#include "ColorConsole.h"

#include "BTAction.h"
#include "Components.h"
#include "GameWorldTime.h"

class BTMoveToRandomPosition : public BTAction
{
public:
	BTMoveToRandomPosition(const std::string& name, GameObject* pawn) : BTAction(name), pawn(pawn)
	{
	}


	BTNodeResult command() override
	{
		using namespace std;
		
		if (!waypoint_assigned)
		{
			int x = 1 + rand() % 30;
			int y = 1 + rand() % 30;


			// Check whether this waypoint collides with any Collidable.
			if (pawn->hasComponent("CollisionBox"))
			{
				for (auto& go : GameObjectStorage::get()->getStorage())
				{
					if (go->hasComponent("CollisionBox"))
					{
						CollisionBoxCmp* coll = pawn->getComponent<CollisionBoxCmp>("CollisionBox");

						if (go->getComponent<CollisionBoxCmp>("CollisionBox")->resolve(x, y, coll->width, coll->height))
						{
							// Collision with this.

							cout << color(colors::RED);
							cout << "[BTMoveToRandomPosition::command()] Waypoint {"<<x << ","<<y << "} invalid. Searching for new one." << white <<endl;

							// Recursion into this function to recreate a waypoint.
							return command();
						}
					}
				}
			}


			pawn->getComponent<NavigatorCmp>("Navigator")->setDestination(x, y);

			waypoint_assigned = true;

			return BTNodeResult::SUCCESS;
		}



		if (_targetReached())
		{
			waypoint_assigned = false;
			return BTNodeResult::SUCCESS;
		}
		else
		{
			return BTNodeResult::RUNNING;
		}
	}



private:
	GameObject* pawn = nullptr;

	bool waypoint_assigned = false;

	bool _targetReached()
	{
		float dt = (float)GameWorldTime::get()->getTimeSpeed();
		return pawn->getComponent<NavigatorCmp>("Navigator")->update(dt);
	}
};