#include "Components.h"

NavMesh* NavMesh::g_NavMesh = nullptr;


bool NavMesh::bake()
{
	graph = new Graph(width, height);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			graph->addNode(i, j);
		}
	}


	for (auto& go : GameObjectStorage::get()->getStorage())
	{

		if (go->getTag().find("Building") == std::string::npos) continue;

		// Start baking the Graph too.
		//
		if (go->hasComponent("CollisionBox"))
		{
			int x, y, w, h;

			x = go->getComponent<TransformCmp>("Transform")->xpos;
			y = go->getComponent<TransformCmp>("Transform")->ypos;

			w = go->getComponent<CollisionBoxCmp>("CollisionBox")->width;
			h = go->getComponent<CollisionBoxCmp>("CollisionBox")->height;


			// Make the whole building solid
			for (int i = x; i < x + w; i++)
			{
				for (int j = y; j < y + h; j++)
				{
					map[i][j] = 99;
					graph->deleteNode(i, j);
				}
			}

			// If the building is walkable
			// Create a doorway node and make inside of building walkable
			if (go->hasComponent("WalkableBuilding"))
			{
				WalkableBuildingCmp* wb = go->getComponent<WalkableBuildingCmp>("WalkableBuilding");
				std::pair<int, int> door = wb->getDoorToBuilding();
				
				for (int i = x + 1; i < x + w - 1; i++)
				{
					for (int j = y + 1; j < y + h - 1; j++)
					{
						map[i][j] = 1;
						graph->addNode(i, j);

						// Find the neighbor of the door and create a node transiting
						// from outside to inside the building

						// Door from left side of building
						if (i - 1 == door.first && j == door.second)
						{
							// Add a node between the door and inner building part
							graph->addNode(door.first, door.second);

							edge a = std::make_tuple(i - 2, j, i - 1, j, 1); // Node from outer to door
							edge b = std::make_tuple(door.first, door.second, i, j, 1); // Node from door to inner

							graph->edges.push_back(a);
							graph->edges.push_back(b);
						}
						// Door from upper side of building
						else if (i == door.first && j + 1 == door.second)
						{
							graph->addNode(door.first, door.second);

							edge a = std::make_tuple(i, j, door.first, door.second, 1); // Node from outer to door
							edge b = std::make_tuple(door.first, door.second, i, j + 2, 1); // Node from door to inner

							graph->edges.push_back(a);
							graph->edges.push_back(b);
						}
						// Door from down side of building
						else if (i == door.first && j - 1 == door.second)
						{
							graph->addNode(door.first, door.second);

							edge a = std::make_tuple(i, j, door.first, door.second, 1); // Node from outer to door
							edge b = std::make_tuple(door.first, door.second, i, j - 2, 1); // Node from door to inner

							graph->edges.push_back(a);
							graph->edges.push_back(b);
						}
						// Door from right side of building
						else if (i + 1 == door.first && j == door.second)
						{
							graph->addNode(door.first, door.second);

							edge a = std::make_tuple(i + 1, j, door.first, door.second, 1); // Node from outer to door
							edge b = std::make_tuple(door.first, door.second, i, j, 1); // Node from door to inner

							graph->edges.push_back(a);
							graph->edges.push_back(b);
						}

						/*
#define OFFSET 1
						if (i - OFFSET == door.first && j == door.second)
						{
							graph->edges.push_back(edge{ i - OFFSET, j, door.first, door.second, 1 });
						}
						else if (i + OFFSET == door.first && j == door.second)
						{
							graph->edges.push_back(edge{ i + OFFSET, j, door.first, door.second, 1 });
						}
						else if (i == door.first && j - OFFSET == door.second)
						{
							graph->edges.push_back(edge{ i, j - OFFSET, door.first, door.second, 1 });
						}
						else if (i == door.first && j + OFFSET == door.second)
						{
							graph->edges.push_back(edge{ i, j + OFFSET, door.first, door.second, 1 });
						}
#undef OFFSET
*/
					}
				}
			}

		}
		else
		{
		}
	}


	for (int i = 0; i < width - 1; i++)
	{
		for (int j = 0; j < height - 1; j++)
		{
			if (graph->nodes[i][j] == 1)
			{
				// We make exactly 2 edges for each node,
				// thus every node has 4 edges in total.

				if (graph->nodes[i + 1][j] == 1)
				{
					edge e = std::make_tuple(i, j, i + 1, j, 1);
					graph->edges.push_back(e);
				}
				if (graph->nodes[i][j + 1] == 1)
				{
					edge e2 = std::make_tuple(i, j, i, j + 1, 1);
					graph->edges.push_back(e2);
				}
			}
		}
	}



	return true;
}





bool CollisionBoxCmp::resolve(GameObject* other)
{
	if (other->hasComponent("Transform"))
	{
		if (other->hasComponent("CollisionBox"))
		{

			if (this_agent->hasComponent("WalkableBuilding"))
			{
				// Special collision detection, where this pawn is the building with a walkable area inside
				// and other is the collision object

				// Thus if the other is inside the collision box AND collides with our agents walkable area, then no collision is detected

				TransformCmp* otr = static_cast<TransformCmp*>(other->getComponent("Transform"));
				CollisionBoxCmp* ocoll = static_cast<CollisionBoxCmp*>(other->getComponent("CollisionBox"));
				TransformCmp* tr = static_cast<TransformCmp*>(this_agent->getComponent("Transform"));

				if (tr->xpos < otr->xpos + ocoll->width &&
					tr->xpos + width > otr->xpos &&
					tr->ypos < otr->ypos + ocoll->height &&
					tr->ypos + height > otr->ypos)
				{
					// Other object is inside collision box of building

					// Reduce dimension of buildings collision box according to walkable area
					int xpos = tr->xpos + 1;
					int ypos = tr->ypos + 1;
					int w = width - 2;
					int h = height - 2;

					// Get the position of the door, which is non-collidable too
					std::pair<int, int> door = this_agent->getComponent<WalkableBuildingCmp>("WalkableBuilding")->getDoorToBuilding();

					if (xpos < otr->xpos + ocoll->width &&
						xpos + w > otr->xpos &&
						ypos < otr->ypos + ocoll->height &&
						ypos + h > otr->ypos)
					{
						// Other object is inside the buildings walkable area --> no collision!
						return false;
					}
					else if (door.first == otr->xpos && door.second == otr->ypos)
					{
						// Other object is on the door --> no collision!
						return false;
					}
					else
					{
						// Other object is NOT inside the buildings walkable area --> collision!
						return true;
					}
				}

			}
			else
			{
				// Plain standard collision detection.
				TransformCmp* otr = static_cast<TransformCmp*>(other->getComponent("Transform"));
				CollisionBoxCmp* ocoll = static_cast<CollisionBoxCmp*>(other->getComponent("CollisionBox"));
				TransformCmp* tr = static_cast<TransformCmp*>(this_agent->getComponent("Transform"));


				if (tr->xpos < otr->xpos + ocoll->width &&
					tr->xpos + width > otr->xpos &&
					tr->ypos < otr->ypos + ocoll->height &&
					tr->ypos + height > otr->ypos)
				{
					return true;
				}
			}
		}
	}

	return false;
}




bool CollisionBoxCmp::resolve(float x, float y, float w, float h)
{
	using namespace std;

	cout << color(colors::CYAN);
	cout << "[CollisionBoxCmp::resolve] resolving Pawn \""<< this_agent->getName() << "\" and Object {x="<< x << ", y="<< y << ", w="<< w << ", h="<< h << "}" << white << endl;

	TransformCmp* transform = static_cast<TransformCmp*>(this_agent->getComponent("Transform"));

	// Resolve collision for this object 
	// If it is a walkable building
	if (this_agent->hasComponent("WalkableBuilding"))
	{
		cout << color(colors::DARKCYAN);
		cout << "\t ...Resolve for Walkable Building..." << white << endl;


		// Check if other object is inside the building dimensions
		if (transform->xpos < x + w &&
			transform->xpos + width > x &&
			transform->ypos < y + h &&
			transform->ypos + height > y)
		{

			cout << color(colors::DARKCYAN);
			cout << "\t\t ...Object is inside the Building Collision Box..." << white << endl;

			// Check whether it is inside the walkable-building inside area
			int xpos = transform->xpos + 1;
			int ypos = transform->ypos + 1;
			int walkable_width = width - 2;
			int walkable_height = height - 2;


			// Get the position of the door, which is non-collidable too
			std::pair<int, int> door = this_agent->getComponent<WalkableBuildingCmp>("WalkableBuilding")->getDoorToBuilding();

			if (xpos < x + w &&
				xpos + walkable_width > x &&
				ypos < y + h &&
				ypos + walkable_height > y)
			{

				cout << color(colors::DARKCYAN);
				cout << "\t\t\t ...Object is inside the Building Walkable Area..." << white << endl;

				// Other object is inside the buildings walkable area --> no collision!
				return false;
			}
			else if (door.first == x && door.second == y)
			{
				cout << color(colors::DARKCYAN);
				cout << "\t\t\t ...Object is on the door..." << white << endl;

				// Other object is on the door --> no collision!
				return false;
			}
			else
			{
				cout << color(colors::DARKCYAN);
				cout << "\t\t\t ...Object is on the wall..." << white << endl;

				// Other object is NOT inside the buildings walkable area --> collision!
				return true;
			}

		}
	}
	// If it is a non-walkable solid object
	else
	{
		cout << color(colors::DARKCYAN);
		cout << "\t ...Resolve for Solid Object..." << white << endl;


		if (transform->xpos < x + w &&
			transform->xpos + width > x &&
			transform->ypos < y + h &&
			transform->ypos + height > y)
		{

			cout << color(colors::DARKCYAN);
			cout << "\t\t ...Object is colliding with solid object..." << white << endl;

			return true;
		}
	}


	cout << color(colors::DARKCYAN);
	cout << "\t\t ...No collision detected..." << white << endl;
	return false;
}