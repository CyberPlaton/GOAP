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
			static bool go_inside_house = false;

			int x = 1 + rand() % 30;
			int y = 1 + rand() % 30;


			if (!go_inside_house)
			{
				// Error on 19, 19 --> It is a Wall!
				x = 19;
				y = 19;
				go_inside_house = true;
			}


			cout << color(colors::YELLOW);
			cout << "Trying Destination {"<< x << ", "<< y << "} for pawn \""<< pawn->getName() <<"\"" << white << endl;


			// Check whether this waypoint collides with any Collidable.
			if (pawn->hasComponent("CollisionBox"))
			{
				for (auto& go : GameObjectStorage::get()->getStorage())
				{
					if (go->hasComponent("CollisionBox"))
					{
						// Do not collide with self.
						if (go->hash == pawn->hash) continue;



						cout << color(colors::YELLOW);
						cout << "Resolving Building collision of \"" << pawn->getName() << "\" and \"" << go->getName() << "\"..." << white;

						CollisionBoxCmp* col = pawn->getComponent<CollisionBoxCmp>("CollisionBox");
						if (go->getComponent<CollisionBoxCmp>("CollisionBox")->resolve(x, y, col->width, col->height)) // Resolve collision with the speculative position of the pawn.
						{
							cout << color(colors::RED);
							cout << "[BTMoveToRandomPosition::command()] Waypoint {" << x << ", " << y << "} invalid. Searching for new one." << white << endl;

							// Recursion into this function to recreate a waypoint.
							return command();
						}

						/*
						// Resolve collision with a building
						if (go->hasComponent("WalkableBuilding"))
						{
							cout << color(colors::YELLOW);
							cout << "Resolving Building collision of Pawn=\""<< pawn->getName() << "\" and Building=\""<< go->getName() << "\" " << white << endl;


							if(go->getComponent<CollisionBoxCmp>("CollisionBox")->resolve(go))
							{
								// Collision with building wall

								cout << color(colors::RED);
								cout << "[BTMoveToRandomPosition::command()] Waypoint {" << x << "," << y << "} invalid as colliding with building wall. Searching for new one." << white << endl;

								// Recursion into this function to recreate a waypoint.
								return command();
							}
						}
						else
						{
							// Resolve collision with a non-building solid object
							CollisionBoxCmp* coll = pawn->getComponent<CollisionBoxCmp>("CollisionBox");
							if (go->getComponent<CollisionBoxCmp>("CollisionBox")->resolve(x, y, coll->width, coll->height))
							{
								// Collision with this.

								cout << color(colors::RED);
								cout << "[BTMoveToRandomPosition::command()] Waypoint {" << x << "," << y << "} invalid. Searching for new one." << white << endl;

								// Recursion into this function to recreate a waypoint.
								return command();
							}
						}
						*/

					}
				}
			}

			cout << color(colors::GREEN);
			cout << "[BTMoveToRandomPosition::command()] Waypoint {" << x << ", " << y << "} Valid!" << white << endl;

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