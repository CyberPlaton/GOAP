#pragma once

#include "ColorConsole.h"
#include "AgentNeeds.h"
#include "AgentRole.h"
#include "AgentStats.h"
#include "OwnershipCmp.h"
#include "WorldState.h"
#include "Inventory.h"
#include "AI.h"

#include "tinyxml2.h"
#include "nlohmann/json.hpp"


class GameObjectCreator
{
public:
	GameObjectCreator(){}

	GameObject* create(const std::string& filepath, const std::string& name, int xpos, int ypos, bool json = false)
	{ 
		return json == true ? _createFromJson(filepath, name, xpos, ypos) : _createFromXML(filepath, name, xpos, ypos);
	}


private:

	GameObject* _createFromXML(const std::string& xml_filepath, const std::string& name, int xpos, int ypos)
	{
		using namespace std;
		using namespace tinyxml2;
		
		// Create an object defined from xml.
		tinyxml2::XMLDocument doc;


		XMLError result = doc.LoadFile(xml_filepath.c_str());
		if (result != XMLError::XML_SUCCESS) return nullptr;


		XMLNode* root = doc.FirstChild();
		if (root == nullptr) return nullptr;


		// Get tag
		std::string tag = root->FirstChildElement("Tag")->GetText();

		// Create the gameobject
		GameObject* gameobject = new GameObject(tag, name);


		/*
		* Get the components defining the agent.
		*/
		XMLElement* components = root->FirstChildElement("Components");

		// Get first component
		XMLElement* cmp = components->FirstChildElement("Component");
		XMLElement* data = nullptr;

		bool panik = false;
		while (cmp)
		{
			// Get the component name
			std::string cmp_name = cmp->Attribute("name");


			// Set data based on the component.
			if (cmp_name.compare("Renderable") == 0)
			{
				float w, h;
				std::string color;

				data = cmp->FirstChildElement("Width");
				w = stod(data->GetText());

				data = cmp->FirstChildElement("Height");
				h = stod(data->GetText());

				data = cmp->FirstChildElement("Color");
				color = data->GetText();


				gameobject->AddComponent(new RendererableCmp("Renderable", w, h, color));
			}
			else if (cmp_name.compare("Transform") == 0)
			{
				gameobject->AddComponent(new TransformCmp("Transform"));
				gameobject->getComponent<TransformCmp>("Transform")->setPosition(xpos, ypos);
			}
			else if (cmp_name.compare("Navigator") == 0)
			{
				gameobject->AddComponent(new NavigatorCmp("Navigator", gameobject));
			}
			else if (cmp_name.compare("CollisionBox") == 0)
			{
				float w, h;
				data = cmp->FirstChildElement("Width");
				w = stod(data->GetText());

				data = cmp->FirstChildElement("Height");
				h = stod(data->GetText());

				gameobject->AddComponent(new CollisionBoxCmp("CollisionBox", w, h, gameobject));
			}
			else if (cmp_name.compare("Needs") == 0)
			{
				gameobject->AddComponent(new AgentNeedsCmp("AgentNeeds"));
				AgentNeedsCmp* needcmp = gameobject->getComponent< AgentNeedsCmp >("AgentNeeds");

				double value;

				// Get Hunger
				data = cmp->FirstChildElement("Hunger");
				value = stod(data->GetText());
				needcmp->setNeed("Hunger", value);

				// Get Thirst
				data = cmp->FirstChildElement("Thirst");
				value = stod(data->GetText());
				needcmp->setNeed("Thirst", value);

				// Get Sleep
				data = cmp->FirstChildElement("Sleep");
				value = stod(data->GetText());
				needcmp->setNeed("Sleep", value);
			}
			else if (cmp_name.compare("Stats") == 0)
			{
				gameobject->AddComponent(new AgentStatsCmp("AgentStats"));
				AgentStatsCmp* statcmp = gameobject->getComponent< AgentStatsCmp >("AgentStats");

				double value;

				// Get Strength
				data = cmp->FirstChildElement("Strength");
				value = stod(data->GetText());
				statcmp->setStrength(value);

				// Get Intellect
				data = cmp->FirstChildElement("Intellect");
				value = stod(data->GetText());
				statcmp->setIntellect(value);

				// Get Willpower
				data = cmp->FirstChildElement("Willpower");
				value = stod(data->GetText());
				statcmp->setWillpower(value);

				// Get Agility
				data = cmp->FirstChildElement("Agility");
				value = stod(data->GetText());
				statcmp->setAgility(value);

				// Get Speed
				data = cmp->FirstChildElement("Speed");
				value = stod(data->GetText());
				statcmp->setSpeed(value);

				// Get Endurance
				data = cmp->FirstChildElement("Endurance");
				value = stod(data->GetText());
				statcmp->setEndurance(value);

				// Get Personality
				data = cmp->FirstChildElement("Personality");
				value = stod(data->GetText());
				statcmp->setPersonality(value);

				// Get Luck
				data = cmp->FirstChildElement("Luck");
				value = stod(data->GetText());
				statcmp->setLuck(value);

				// Get Health
				data = cmp->FirstChildElement("Health");
				value = stod(data->GetText());
				statcmp->setHealth(value);

				// Get Magicka
				data = cmp->FirstChildElement("Magicka");
				value = stod(data->GetText());
				statcmp->setMagicka(value);

				// Get Fatigue
				data = cmp->FirstChildElement("Fatigue");
				value = stod(data->GetText());
				statcmp->setFatigue(value);
			}
			else if (cmp_name.compare("Inventory") == 0)
			{
				gameobject->AddComponent(new InventoryCmp("Inventory"));
			}
			else if (cmp_name.compare("WorldState") == 0)
			{
				gameobject->AddComponent(new WorldStateCmp("WorldState"));
			}
			else if (cmp_name.compare("Animator") == 0)
			{
				gameobject->AddComponent(new AnimatorCmp("Animator", gameobject));
			}
			else if (cmp_name.compare("Ownership") == 0)
			{
				gameobject->AddComponent(new OwnershipCmp("Ownership", gameobject->getTag()));


				// Check whether there are children objects defined which are owned by gameobject
				XMLElement* kid = cmp->FirstChildElement("Gameobject");
				XMLElement* kid_data = nullptr;
				while (kid)
				{
					// It exists at least one child object.

					// Get data.
					int kidx = stod(kid->Attribute("posx"));
					int kidy = stod(kid->Attribute("posy"));
					std::string kidname = kid->Attribute("name");
					
					// Create the child Gameobject.
					std::string path = "GOAP/Gameobjects/";
					std::string childpath = kid->GetText();
					path += childpath; // Get the path.

					GameObjectCreator creator;
					GameObject* kidobject = creator.create(path, name + "_" + kidname, xpos + kidx, ypos + kidy);
					if (kidobject)
					{
						gameobject->getComponent<OwnershipCmp>("Ownership")->addGameobject(kidobject);
					}
					else
					{
						cout << color(colors::RED);
						cout << "Could not create owned gameobject for \""<< gameobject->getName() << "\" at path \""<< path << "\"" << white << endl;
					}

					kid = kid->NextSiblingElement("Gameobject");
				}

			}
			else if (cmp_name.compare("WalkableBuilding") == 0)
			{
				int doorx = (int)stod(cmp->Attribute("doorx"));
				int doory = (int)stod(cmp->Attribute("doory"));

				gameobject->AddComponent(new WalkableBuildingCmp("WalkableBuilding", gameobject , doorx, doory));
			}



			if (panik)
			{
				cout << color(colors::RED);
				cout << "Could not construct Gameobject \"" << name << "\"" << white << endl;

				delete gameobject;
				gameobject = nullptr;
				break;
			}


			cmp = cmp->NextSiblingElement("Component");
		}


		doc.Clear();
		return gameobject;
	}


	GameObject* _createFromJson(const std::string& json_filepath, const std::string& name, int xpos, int ypos)
	{
		// Not implemented
		return nullptr;
	}

	/*
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

			
			for (auto& e : in.at("Layout"))
			{
				std::string jsonpath = "GOAP/Gameobjects/" + e.at("Name").get<std::string>() + ".json";

				// Specify the name for the child.
				std::string child_name = name + " " + e.at("Name").get<std::string>();

				GameObject* obj = create(jsonpath, child_name, 0, 0); // Create children without specific position.

				// Children Objects do have position entry.
				int obj_xpos = e.at("Position")[0].get<int>();
				int obj_ypos = e.at("Position")[1].get<int>();

				obj->setPosition(xpos + obj_xpos, ypos + obj_ypos); // Set the relative position.
			}


			cout << color(colors::YELLOW);
			cout << "[BUILDING] NEW BUILDING: ";
			cout << "Tag \"";
			cout << color(colors::GREEN);
			cout << building->getTag() << "\"";

			cout << color(colors::YELLOW);
			cout << "Name ";
			cout << color(colors::GREEN);
			cout << building->getName() << "\"" << white << endl;

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

					// Furthermore, ownership extends on all initial gameobjects inside the building,
					// if owned_object is a building.
					if (owned_thing->getTag().find("Building") != std::string::npos)
					{
						// Get all GOs, which are INSIDE it,
						// we can easily get those by parent-child naming convention.
						for (auto& go : GameObjectStorage::get()->getStorage())
						{
							// Dont pushback the buildings themselves a second time.
							if (go->getName().compare(owned_thing->getName()) == 0) continue;


							if (go->getName().find(owned_thing->getName()) != std::string::npos)
							{
								npc->agentOwnedObjects.push_back(go);

								cout << "[OWNERSHIP] Object found:  \"" << go->getName() << "\"";
								cout << ", \"" << name << "\" now owns it!" << endl;
							}
						}
					}
				}
			}


			cout << color(colors::YELLOW);
			cout << "[NPC] NEW NPC: ";
			cout << "Tag \"";
			cout << color(colors::GREEN);
			cout << npc->getTag() << "\"";

			cout << color(colors::YELLOW);
			cout << "Name ";
			cout << color(colors::GREEN);
			cout << npc->getName() << "\"" << white << endl;

			return npc;
		}
		else if (type.compare("Furniture") == 0)
		{
			GameObject* furniture = new GameObject("Furniture", name);

			furniture->AddComponent(new TransformCmp(furniture->tag + "_Transform"));
			
			// Position of the furniture is defined from the parent gameobject, e.g. a house.

			furniture->AddComponent(new RendererableCmp(furniture->tag + "_Renderable", in.at("Width").get<int>(), in.at("Height").get<int>(), "dark_grey"));



			cout << color(colors::YELLOW);
			cout << "[FURNITURE] NEW FURNITURE: ";
			cout << "Tag \"";
			cout << color(colors::GREEN);
			cout << furniture->getTag() << "\"";

			cout << color(colors::YELLOW);
			cout << "Name ";
			cout << color(colors::GREEN);
			cout << furniture->getName() << "\"" << white << endl;


			return furniture;
		}

		return nullptr;
	}
	*/
};