#pragma once

#include "Agent.h"

#include "nlohmann/json.hpp"


class GameObjectCreator
{
public:
	GameObjectCreator(){}

	GameObject* create(const std::string& filepath, const std::string& name, int xpos, int ypos) { return _createFromJson(filepath, name, xpos, ypos); }




private:

	GameObject* _createFromJson(const std::string& json_filepath, const std::string& name, int xpos, int ypos)
	{
		using namespace std;
		using namespace nlohmann;

		ifstream fin(json_filepath);
		if (fin.is_open() == false) return nullptr;

		json in;
		fin >> in;


		std::string type = in.at("GameObjectType").get<std::string>();


		if (type.compare("Building") == 0)
		{
			GameObject* building = new GameObject("Building", name);

			building->AddComponent(new TransformCmp(building->tag + "_Transform"));

			building->setPosition(xpos, ypos);
			 

			building->AddComponent(new RendererableCmp(building->tag + "_Renderable", in.at("Width").get<int>(), in.at("Height").get<int>(), "grey"));

			/*
			* Layout defines the objects within the building,
			* thus alowing to specify it from json.
			* 
			* Hint: all child object positions are relative to parent, thus this building.
			*/
			for (auto& e : in.at("Layout"))
			{
				std::string jsonpath = "GOAP/Gameobjects/" + e.at("Name").get<std::string>() + ".json";

				// Specify the name for the child.
				std::string child_name = name + "_" + e.at("Name").get<std::string>();

				GameObject* obj = create(jsonpath, child_name, 0, 0); // Create children without specific position.

				// Children Objects do have position entry.
				int obj_xpos = e.at("Position")[0].get<int>();
				int obj_ypos = e.at("Position")[1].get<int>();

				obj->setPosition(xpos + obj_xpos, ypos + obj_ypos); // Set the relative position.
			}


			return building;
		}
		else if (type.compare("NPC") == 0)
		{
			Agent* npc = new Agent("NPC", name);
			npc->awake();

			npc->AddComponent(new TransformCmp(npc->tag + "_Transform"));

			npc->setPosition(xpos, ypos);

			npc->AddComponent(new RendererableCmp(npc->tag + "_Renderable", 0.8f, 0.8f, "green"));


			// An NPC is initialized at some position,
			// he can have some objects in his inventory and
			// he can possess some objects (In order to possess them, they MUST be created first).

			for (auto& e : in.at("Ownership"))
			{
				GameObject* owned_thing = GameObjectStorage::get()->getGOByName(e.at("Name").get<std::string>());
			

				if(!owned_thing)
				{
					cout << "[OWNERSHIP] Object not found:  " << e.at("Name").get<std::string>() << endl;
				}
				else
				{
					cout << "[OWNERSHIP] Object found:  \"" << e.at("Name").get<std::string>() << "\"";
					cout << ", \"" << name << "\" now owns it!" << endl;

					npc->agentOwnedObjects.push_back(owned_thing);
				}
			}



			return npc;
		}
		else if (type.compare("Furniture") == 0)
		{
			GameObject* furniture = new GameObject("Furniture", name);

			furniture->AddComponent(new TransformCmp(furniture->tag + "_Transform"));
			
			// Position of the furniture is defined from the parent gameobject, e.g. a house.

			furniture->AddComponent(new RendererableCmp(furniture->tag + "_Renderable", in.at("Width").get<int>(), in.at("Height").get<int>(), "dark_grey"));

			return furniture;
		}

		return nullptr;
	}

};