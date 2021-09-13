#pragma once

#include "ColorConsole.h"

#include "GameObject.h"
#include "ComponentSystem.h"


#include <stack>
#include <queue>


class CollisionBoxCmp : public Component
{
public:
	CollisionBoxCmp(const ComponentID& name, float width, float height, GameObject* go) :
		width(width), height(height), this_agent(go)
	{
		this->name = name;
		type = "CollisionBox";

		init(type);
	}

	ComponentType getType() override { return this->type; }

	bool resolve(GameObject* other)
	{
		if (other->hasComponent("Transform"))
		{
			if (other->hasComponent("CollisionBox"))
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

		return false;
	}



	bool resolve(float x, float y, float w, float h)
	{
		TransformCmp* tr = static_cast<TransformCmp*>(this_agent->getComponent("Transform"));

		if (tr->xpos < x + w &&
			tr->xpos + width > x &&
			tr->ypos < y + h &&
			tr->ypos + height > y)
		{
			return true;
		}

		return false;
	}


	GameObject* this_agent = nullptr;

	ComponentType type;

	float width = 0.0f;
	float height = 0.0f;
};




//					From x, y    To x, y,   Weight.
using edge = std::tuple<int, int, int, int, float>;
using node = std::tuple<int, int>;

#define node_x(n) std::get<0>(n)
#define node_y(n) std::get<1>(n)

#define edge_from_x(e) std::get<0>(e)
#define edge_from_y(e) std::get<1>(e)

#define edge_to_x(e) std::get<2>(e)
#define edge_to_y(e) std::get<3>(e)

#define edge_weight(e) std::get<4>(e)

struct Graph
{
	Graph(int width, int height)
	{
		nodes.resize(width);

		for (int i = 0; i < height; i++)
		{
			nodes[i].resize(height);
		}

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				nodes[i][j] = 0; // No nodes in graph.
			}
		}
	}


	/*
	* Get weight of edge [a][aa]-----[b][bb].
	*							  ^
	*							  |
	*						edge --
	*/
	float getWeight(node from, node to)
	{
		return getWeight(node_x(from), node_y(from), node_x(to), node_y(to));
	}

	float getWeight(int a, int aa, int b, int bb)
	{
		for (auto& e : edges)
		{
			int x = edge_from_x(e);
			int xx = edge_from_y(e);
			int y = edge_to_x(e);
			int yy = edge_to_y(e);

			int w = edge_weight(e);

			// Edges are undirected. Thus check for both cases.
			if (a == x && aa == xx && y == b && yy == bb ||
				b == x && bb == xx && y == a && yy == aa) return w;
		}

		return INT_MAX;
	}


	/*
	* Set the weight of an edge.
	*/
	void setWeight(node x, node y, float w)
	{
		setWeight(node_x(x), node_y(x), node_x(y), node_y(y), w);
	}

	void setWeight(int a, int aa, int b, int bb, float w)
	{
		int index = 0;
		for (auto& e : edges)
		{
			int x = edge_from_x(e);
			int xx = edge_from_y(e);
			int y = edge_to_x(e);
			int yy = edge_to_y(e);


			if (a == x && aa == xx && y == b && yy == bb ||
				b == x && bb == xx && y == a && yy == aa)
			{
				index++;

				edges[index] = std::make_tuple(x, xx, y, yy, w);
				return;
			}

			index++;
		}
	}


	std::vector<node> getNeighbors(node n)
	{
		return getNeighbors(node_x(n), node_y(n));
	}

	std::vector<node> getNeighbors(int x, int y)
	{
		std::vector<std::tuple<int, int>> neighbors;

		for (auto& e : edges)
		{
			int fromx = edge_from_x(e);
			int fromy = edge_from_y(e);
			int tox = edge_to_x(e);
			int toy = edge_to_y(e);

			if (tox == x && toy == y)
			{
				neighbors.push_back({ fromx, fromy });
			}

			if (fromx == x && fromy == y)
			{
				neighbors.push_back({tox, toy});
			}
		}

		return neighbors;
	}


	void addNode(int x, int y)
	{
		nodes[x][y] = 1;
	}

	void deleteNode(int x, int y)
	{
		nodes[x][y] = 0;
	}

	std::vector<std::vector<int>> nodes;
	std::vector< edge > edges;
};


/*
* Not a component, but still very important.
*/
class NavMesh
{
public:

	static NavMesh* get()
	{
		if (!g_NavMesh)
		{
			g_NavMesh = new NavMesh(32, 32);
		}

		return g_NavMesh;
	}

	static void del()
	{
		if (g_NavMesh)
		{
			delete g_NavMesh;
		}
	}


	bool bake()
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



				for (int i = x; i < x + w; i++)
				{
					for (int j = y; j < y + h; j++)
					{
						map[i][j] = 99;
						graph->deleteNode(i, j);
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




	std::vector<std::vector<int>> getNavMesh() { return map; }
	std::vector<std::vector<int>> getNavGraph() { return graph->nodes; }
	Graph* getGraph() { return graph; }
	int getWidth() { return width; }
	int getHeight() { return height; }


private:

	NavMesh(int width, int height) : width(width), height(height)
	{
		map.resize(width);

		for (int i = 0; i < height; i++)
		{
			map[i].resize(height);
		}

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				map[i][j] = 1;
			}
		}
	}



	static NavMesh* g_NavMesh;

	std::vector<std::vector<int>> map;

	Graph* graph = nullptr;

	int width, height;
};



class NavigatorCmp : public Component
{
public:

	NavigatorCmp(const ComponentID& name, GameObject* go) : agent(go)
	{
		this->name = name;
		type = "Navigator";

		init(type);
	}


	ComponentType getType() override { return this->type; }


	/*
	* Set the next waypoint/moving dest. for a gameobject.
	*/
	void setDestination(float x, float y)
	{
		destx = x;
		desty = y;
	}


	/*
	* General update function.
	* Returns true if destination was reached.
	*/
	bool update(float dt)
	{
		using namespace std;

		if (Component* cmp = agent->getComponent("Transform"); cmp != nullptr)
		{
			TransformCmp* t = static_cast<TransformCmp*>(cmp);

			// Is Target reached
			if (t->xpos == destx && t->ypos == desty)
			{
				movementPoints.clear();
				curr_mp_index = 0;
				return true;
			}


			if (movementPoints.size() == 0)
			{
				movementPoints = this->_getPathToDest(t->xpos, t->ypos, destx, desty, NavMesh::get()->getGraph());


				if (movementPoints.size() > 0)
				{
				}
				else
				{
				}
			}
			else if (movementPoints.size() > 0)
			{
				if (_updateWaypointMovement(dt, t) == false) return false;
			}


			// Component update, if needed.
			if (internal_agent_positionx > 1.0f)
			{
				t->xpos += 1;
				internal_agent_positionx = 0.0f;
			}
			else if (internal_agent_positionx < -1.0f)
			{
				t->xpos -= 1;
				internal_agent_positionx = 0.0f;
			}


			if (internal_agent_positiony > 1.0f)
			{
				t->ypos += 1;
				internal_agent_positiony = 0.0f;
			}
			else if (internal_agent_positiony < -1.0f)
			{
				t->ypos -= 1;
				internal_agent_positiony = 0.0f;
			}

			return false;
		}

		return false;
	}


	GameObject* agent = nullptr;

	ComponentType type;

	float destx = 0.0f;
	float desty = 0.0f;

	/*
	* Used to count in floats the agents position,
	* and then update the actuall transform component.
	*/
	float internal_agent_positionx = 0.0f;
	float internal_agent_positiony = 0.0f;


	std::vector<node> movementPoints;
	int curr_mp_index = 0;
	float curr_mpx;
	float curr_mpy;


	bool _updateWaypointMovement(float dt, TransformCmp* t)
	{
		// Get next waypoint.
		curr_mpx = std::get<0>(movementPoints[curr_mp_index]);
		curr_mpy = std::get<1>(movementPoints[curr_mp_index]);

		// Move to waypoint.
		// In movement to waypoint we do not have to check for obstacles.
		if (t->xpos < curr_mpx)
		{
			internal_agent_positionx += dt;
		}

		else if (t->xpos > curr_mpx)
		{
			internal_agent_positionx -= dt;
		}


		if (t->ypos < curr_mpy)
		{
			internal_agent_positiony += dt;
		}
		else if (t->ypos > curr_mpy)
		{
			internal_agent_positiony -= dt;
		}

		// Check whether waypoint reached and needs to be updated.
		if (t->xpos == curr_mpx && t->ypos == curr_mpy)
		{
			curr_mp_index++;
		}

		return true;
	}


	/*
	* Implementing the A*-Algorithm.
	*/
	std::vector<node> _getPathToDest(int startx, int starty, int x, int y, Graph* graph)
	{
		using namespace std;
		bool path_found = false;
		std::vector<node> path;

		// Distance between some node and starting node.
		std::map<node, float> g;

		// Heuristic, estimated distance from some node to end point.
		std::map<node, float> h;

		// Total cost for some node.
		std::map<node, float> f;

		// Parent node for some node, meaning, it was visited before visiting the accessed node.
		std::map<node, node> parent_map;

		// Initialization
		std::vector<node> open_list;
		open_list.push_back({ startx, starty });
		f[{startx, starty}] = 0.0f;
		
		std::vector<node> closed_list;
		std::vector<node> nearest_node_neighbors;



		while (!open_list.empty())
		{
			// Get current node.
			node nearest = _getNodeWithLowestCost(open_list, f);
			_removeNode(nearest, open_list);
			closed_list.push_back(nearest);


			if (_isNodeDestinationNode(nearest, { x, y }))
			{

				path_found = true;
				break;
			}
			else
			{
				// Get the neighbors for current node.
				nearest_node_neighbors = graph->getNeighbors(nearest);


				// Examine neighbors of current node.
				for (auto& n : nearest_node_neighbors)
				{
					// Continue if n is in closed, meaning was visited already.
					if (_nodeInList(n, closed_list)) continue;


					// Make "current" node parent of "this" node
					parent_map[n] = nearest;


					// Compute f, g, h value for node.
					g[n] = g[n] + _manhattenDistance(nearest, n);
					h[n] = _manhattenDistance(n, { x, y });
					f[n] = g[n] + h[n];



					if (_nodeInList(n, open_list))
					{
						for (auto& node : open_list)
						{
							if (g[n] > g[node]) continue;
						}
					}


					open_list.push_back(n);
				}
			}
		}


		std::vector<node> temp;

		if (!path_found)
		{
			path.clear();
		}
		else
		{
			// Reconstruct path.
			node child_node = node(x, y); // Initial node.
			temp.push_back(child_node);

			while (true)
			{
				try
				{
					node parent_of_node = parent_map.at(child_node);

					temp.push_back(parent_of_node);

					child_node = parent_of_node;
				}
				catch (...)
				{
					break;
				}
			}

			// Reverse path to be correct.
			for (int i = temp.size() - 1; i >= 0; i--)
			{
				path.push_back(temp[i]);
			}

			//path = closed_list;
		}


		return path;
	}

	bool _nodeInList(node n, std::vector<node>& v)
	{
		std::vector<node>::iterator it = std::find(v.begin(), v.end(), n);
		if (it != v.end())
		{
			return true;
		}

		return false;
	}

	void _removeNode(node n, std::vector<node>& v)
	{
		std::vector<node>::iterator it = std::find(v.begin(), v.end(), n);
		if (it != v.end())
		{
			v.erase(it);
		}
	}


	/*
	* Check whether given node is our destination node.
	*/
	bool _isNodeDestinationNode(node x, node dest)
	{
		if (node_x(x) == node_x(dest) &&
			node_y(x) == node_y(dest))
		{
			return true;
		}

		return false;
	}

	/*
	* Returns the node with smallest distance to destination.
	*/
	node _getNodeWithLowestCost(std::vector<node>& v, std::map<node, float>& map)
	{
		float min = (float)INT_MAX;
		node node(-1, -1);

		for (auto& n : v)
		{
			float d = map[n];

			if (d < min)
			{
				min = d;
				node = n;
			}
		}

		return node;
	}


	/*
	* Compute the manhatten distance from node x to node y.
	*/
	float _manhattenDistance(node x, node y)
	{
		float xx, xy, yx, yy;
		xx = node_x(x);
		xy = node_y(x);
		yx = node_x(y);
		yy = node_y(y);

		// Vectorial 2D-Distance.
		return abs(xx - yx) + abs(xy - yy);
	}

};