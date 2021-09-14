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