#pragma once

#include "Agent.h"

#include "nlohmann/json.hpp"


class GameObjectCreator
{
public:
	GameObjectCreator(){}

	GameObject* create(const std::string& filepath) { return _createFromJson(filepath); }




private:

	GameObject* _createFromJson(const std::string& json_filepath)
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
			GameObject* building = new GameObject(in.at("Name").get<std::string>());

			building->AddComponent(new TransformCmp(building->tag + "_Transform"));

			int xpos, ypos;
			xpos = in.at("Position")[0].get<int>();
			ypos = in.at("Position")[1].get<int>();


			cout << "[BUILDING] Setting Parent Position: " << xpos << ", " << ypos << endl;
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
				GameObject* obj = create(jsonpath);

				int obj_xpos = e.at("Position")[0].get<int>();
				int obj_ypos = e.at("Position")[1].get<int>();

				cout << "[BUILDING] Setting Child Position: " << xpos + obj_xpos << ", " << ypos + obj_ypos << endl;
				obj->setPosition(xpos + obj_xpos, ypos + obj_ypos); // Set the relative position.
			}


			return building;
		}
		else if (type.compare("NPC") == 0)
		{
			
		}
		else if (type.compare("Furniture") == 0)
		{
			GameObject* furniture = new GameObject(in.at("Name").get<std::string>());

			furniture->AddComponent(new TransformCmp(furniture->tag + "_Transform"));
			
			// Position of the furniture is defined from the parent gameobject, e.g. a house.

			furniture->AddComponent(new RendererableCmp(furniture->tag + "_Renderable", in.at("Width").get<int>(), in.at("Height").get<int>(), "dark_grey"));

			return furniture;
		}

		return nullptr;
	}

};