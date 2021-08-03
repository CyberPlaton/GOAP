#include "main.h"
#include "GameObject.h"


/*
* IMGUI related variables, for easier iterating.
*/
static bool imgui_demo_window = false;
static bool gameobjects_window = true;
static bool imgui_has_focus = false;
static const char* selectable_agent_stats[] = { "Stats", "Agent Beliefs", "Goals", "Available Actions", "Action Queue", "Agent Schedule"};
static int show_agent_stats_at_index = -1;
static Agent* agent_showing_stats = nullptr;
static bool show_agent_stats_window = false;

/*
* Helper, maybe GUI related vars.
*/
static GameObject* selected_gameobject = nullptr;


void App::DrawUI(void)
{
	//This finishes the Dear ImGui and renders it to the screen
	pge_imgui.ImGui_ImplPGE_Render();
}


bool App::OnUserUpdate(float fElapsedTime)
{
	using namespace std;

	Clear(olc::BLACK);

	_handleInput(); // Handle the users input.
	GameWorldTime::get()->update(); // Update Game World Time.

	// Update all relevant gameobjects.
	for (auto& go : GameObjectStorage::get()->getStorage())
	{
		if (go->getTag().find("NPC") != std::string::npos)
		{
			Agent* npc = static_cast<Agent*>(go);

			npc->update();
		}
	}


	// Application rendering.
	olc::vi2d topleft = tv.GetTopLeftTile().max({ 0, 0 });
	olc::vi2d bottomright = tv.GetBottomRightTile().min({32, 32});
	olc::vi2d tile;


	// Draw Grid.
	for (tile.y = topleft.y; tile.y < bottomright.y; tile.y++)
	{
		for (tile.x = topleft.x; tile.x < bottomright.x; tile.x++)
		{
			tv.DrawLine(tile, tile + olc::vf2d(0.0f, 1.0f), olc::VERY_DARK_GREY);
			tv.DrawLine(tile, tile + olc::vf2d(1.0f, 0.0f), olc::VERY_DARK_GREY);
		}
	}




	for (auto& go : GameObjectStorage::get()->getStorage())
	{
		olc::Pixel color;

		// Just to get color string and to render it then we need O(n*n), which is baad.
		if (go->hasComponent("Renderable"))
		{
			TransformCmp* tr = static_cast<TransformCmp*> (go->getComponent("Transform"));
			RendererableCmp* rc = static_cast<RendererableCmp*> (go->getComponent("Renderable"));

			color = _getColorFromString(rc->color);

			if (rc->render)
			{
				tv.FillRectDecal(olc::vf2d(tr->xpos, tr->ypos), olc::vf2d(rc->width, rc->height), color);
			}
		}
	}


	if (selected_gameobject)
	{
		TransformCmp* tr = static_cast<TransformCmp*> (selected_gameobject->getComponent("Transform"));
		RendererableCmp* rc = static_cast<RendererableCmp*> (selected_gameobject->getComponent("Renderable"));

		if (tr != nullptr && rc != nullptr)
		{
			olc::vf2d p = { tr->xpos + rc->width / 2.0f - 0.3f, tr->ypos + rc->height / 2.0f };
			tv.DrawStringDecal(p, selected_gameobject->tag, olc::RED, olc::vf2d(0.5f, 0.5f));
		}
	}



	// For Rendering IMGUI.
	_onImGui();
	
	return true;
}


bool App::OnUserCreate()
{
	using namespace std;

	m_GameLayer = CreateLayer();
	EnableLayer(m_GameLayer, true);
	SetLayerCustomRenderFunction(0, std::bind(&App::DrawUI, this));


	tv = olc::TileTransformedView({ ScreenWidth(), ScreenHeight() }, {32, 32});


	GameWorldTime::get()->setTimeSpeed(0.001);

	Agent::addRoleDefinitionPath("Innkeeper", "GOAP/Schedules/schedule_innkeeper.json");


	GameObjectCreator creator;

	GameObject* tavern = creator.create("GOAP/Gameobjects/Tavern.json", "Village Tavern", 3, 5);
	GameObject* shop = creator.create("GOAP/Gameobjects/Shop.json", "Filthy Shop", 13, 5);
	GameObject* house = creator.create("GOAP/Gameobjects/House.json", "Johns House", 21, 2);
	house = creator.create("GOAP/Gameobjects/House.json", "Asmons House", 25, 3);
	house = creator.create("GOAP/Gameobjects/House.json", "Walters House", 28, 8);
	house = creator.create("GOAP/Gameobjects/House.json", "Marthas House", 22, 16);


	GameObject* npc = creator.create("GOAP/Gameobjects/Innkeeper.json", "John", 0, 0);

	// Initialization
	static_cast<Agent*>(npc)->awake(); // Initialize inner workings
	static_cast<Agent*>(npc)->init("GOAP/available_actions.json"); // Initialize available actions.
	static_cast<Agent*>(npc)->assignRole("Innkeeper"); // Assign a role to the npc.
	


	return true;
}



int main()
{
	App demo;
	if (demo.Construct(350, 230, 4, 4))
		demo.Start();
	return 0;
}


void App::_onImGui()
{
	using namespace std;

	SetDrawTarget((uint8_t)m_GameLayer);

	selected_gameobject = nullptr;


	// CHECK WHETHER IMGUI IS FOCUSED
	if (ImGui::IsAnyItemFocused() || ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive() || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	{
		imgui_has_focus = true;
	}
	else
	{
		imgui_has_focus = false;
	}

	// DISPLAY GAMEWORLD TIME
	double day = GameWorldTime::get()->getDay();
	double week = GameWorldTime::get()->getWeek();
	double month = GameWorldTime::get()->getMonth();
	double year = GameWorldTime::get()->getYear();

	std::string worldtime = "TIME:" + GameWorldTime::get()->getDaytimeString();
	worldtime += " D: " + std::to_string(day);
	worldtime += " W: " + std::to_string(week);
	worldtime += " M: " + std::to_string(month);
	worldtime += " Y: " + std::to_string(year);

	DrawStringDecal(olc::vf2d(ScreenWidth() / 2.0f - ScreenWidth() / 4.0f, 5.0f), worldtime, olc::RED, olc::vf2d(0.5f, 0.5f));

	// DEMO
	if (imgui_demo_window)
	{
		ImGui::ShowDemoWindow(&imgui_demo_window);
	}


	// MAIN MENU BAR
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{

			if (ImGui::BeginMenu("Time Speed"))
			{
				float speed = GameWorldTime::get()->getTimeSpeed();
				ImGui::SliderFloat("Timespeed", &speed, 0.0f, 2.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
				GameWorldTime::get()->setTimeSpeed(speed);

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}


	// GAMEOBJECTS WINDOW
	int go_count = GameObjectStorage::get()->getStorage().size();
	ImGui::SetNextWindowPos(ImVec2(1.0f, 15.0f), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(350.0f, 600.0f), ImGuiCond_Appearing);
	if (ImGui::Begin("GameObjects", &gameobjects_window))
	{
		for (auto& go : GameObjectStorage::get()->getStorage())
		{
			// Show GO
			std::string tag_name = go->getTag() + " \"" + go->getName() +"\"";
			bool ret = ImGui::CollapsingHeader(tag_name.c_str());

			// Check whether we are hovering over the current displayed GO.
			if (ImGui::IsItemHovered())
			{
				selected_gameobject = go;
			}

			// Show the components of Selected GO.
			if (ret)
			{
				Agent* npc = static_cast<Agent*>(go);


				// If GO is an NPC, show ownership and inventory.
				if(go->getTag().find("NPC") != std::string::npos)
				{
					// Allow opening NPC related popups.
					if (ImGui::Button("Statistics"))
					{
						ImGui::OpenPopup("npc_stats_popup");
					}
					if (ImGui::BeginPopup("npc_stats_popup"))
					{
						ImGui::Text("Statistics Windows");
						ImGui::Separator();

						// Show possible stats and stuff windows for NPC.
						for (int i = 0; i < IM_ARRAYSIZE(selectable_agent_stats); i++)
						{
							if (ImGui::Selectable(selectable_agent_stats[i]))
							{
								show_agent_stats_at_index = i;

								// Save the agent for which to show stats.
								agent_showing_stats = npc;

								// Open the window.
								show_agent_stats_window = true;

								break;
							}
						}

						ImGui::EndPopup();
					}


					if (ImGui::TreeNode("Ownership"))
					{
						for (auto& item : npc->agentOwnedObjects)
						{
							ImGui::BulletText(item->getName().c_str());
						}

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Inventory"))
					{
						for (auto& item : npc->agentInventory->getItems())
						{
							ImGui::BulletText(item->getName().c_str());
						}

						ImGui::TreePop();
					}
				}


				// Show CMPs
				for (auto& cmp : go->components)
				{
					if (ImGui::TreeNode(cmp->name.c_str()))
					{
						if (cmp->getType().find("Transform") != std::string::npos)
						{
							int v[2];
							v[0] = static_cast<TransformCmp*>(cmp)->xpos;
							v[1] = static_cast<TransformCmp*>(cmp)->ypos;

							if (ImGui::SliderInt2("Position", v, -32, 32))
							{
								static_cast<TransformCmp*>(cmp)->xpos = v[0];
								static_cast<TransformCmp*>(cmp)->ypos = v[1];
							}
						}


						if (cmp->getType().find("Renderable") != std::string::npos)
						{
							RendererableCmp* rc = static_cast<RendererableCmp*>(cmp);

							ImGui::Text(rc->color.c_str());

							ImGui::SameLine();

							ImGui::Checkbox("Render", &rc->render);
						}

						ImGui::TreePop();
					}

					ImGui::Separator();
				}
			}
		}
	}
	ImGui::End();


	/*
	* Currently the implementation allows only one of the stats windows to be active.
	*/
	if (agent_showing_stats != nullptr)
	{
		if (show_agent_stats_window)
		{
			ImGui::SetNextWindowSize(ImVec2(400.0f, 250.0f), ImGuiCond_Appearing);
			ImGui::SetNextWindowPos(ImVec2(350.0f, 15.0f), ImGuiCond_Appearing);
			switch (show_agent_stats_at_index)
			{
			case 0:
				_imguiAgentStatsWindow();
				break;

			case 1:
				_imguiAgentBeliefsWindow();
				break;

			case 2:
				_imguiAgentGoalsWindow();
				break;

			case 3:
				_imguiAgentAvailableActionsWindow();
				break;

			case 4:
				_imguiAgentActionQueueWindow();
				break;

			case 5:
				_imguiAgentScheduleWindow();
				break;

			default:	// Show nothing.
				break;
			}
		}
	}
}


void App::_imguiAgentScheduleWindow()
{
	if (ImGui::Begin(selectable_agent_stats[show_agent_stats_at_index], &show_agent_stats_window))
	{
		ImGui::Text("Role: ");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("The role of an agent defines his daily schedule");

		ImGui::SameLine();
		ImGui::Text(agent_showing_stats->agentRole.c_str());
		ImGui::Separator();

		for (auto& schedule_entry : agent_showing_stats->daySchedule->schedule)
		{
			/*
			* Role: \"Beggar\"
			* Activity: \"Sleep\"
			* Target: \"Bed\"
			* Start: 11.30
			* End: 7.30
			*/

			ImGui::Text("Activity: ");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("The name of the activity");

			ImGui::SameLine();
			ImGui::Text(schedule_entry->name.c_str());


			ImGui::Text("Target: ");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Tag of the Gameobject where the activity takes place");

			ImGui::SameLine();
			ImGui::Text("TODO");



			ImGui::Text("Start: ");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("When the activity will start");

			ImGui::SameLine();
			ImGui::Text(std::to_string(schedule_entry->start).c_str());



			ImGui::Text("End: ");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("When the activity will end");

			ImGui::SameLine();
			ImGui::Text(std::to_string(schedule_entry->end).c_str());

			ImGui::Separator();
		}
	}

	ImGui::End();
}

void App::_imguiAgentStatsWindow()
{
	if (ImGui::Begin(selectable_agent_stats[show_agent_stats_at_index], &show_agent_stats_window))
	{
		ImGui::Text("Stats");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Here we show the stats of an NPC like the race, agility or strength, health points etc");
	
	}

	ImGui::End();
}

void App::_imguiAgentBeliefsWindow()
{
	if (ImGui::Begin(selectable_agent_stats[show_agent_stats_at_index], &show_agent_stats_window))
	{
		ImGui::Text("Beliefs");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Here we show the agents local world state, so called beliefs, meaning how he perceives the world around self");
	
	}

	ImGui::End();
}


void App::_imguiAgentGoalsWindow()
{
	if (ImGui::Begin(selectable_agent_stats[show_agent_stats_at_index], &show_agent_stats_window))
	{
		ImGui::Text("Goals");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Here we show the goals of the agent in a comprehensive way");
		ImGui::Separator();


		for (auto& g : agent_showing_stats->goals)
		{

			ImGui::Text("Goal Definition with ");
			ImGui::SameLine();
			ImGui::Text("Priority: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(g.first).c_str());

			for (auto& sg : g.second->goals)
			{
				ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Subgoal: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), sg.second.c_str());

				ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Priority: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), std::to_string(sg.first).c_str());
			}

		
			ImGui::Separator();
		}
	}

	ImGui::End();
}


void App::_imguiAgentAvailableActionsWindow()
{
	if (ImGui::Begin(selectable_agent_stats[show_agent_stats_at_index], &show_agent_stats_window))
	{
		ImGui::Text("Available Actions");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Here we show all actions available for execution for this agent");
	}

	ImGui::End();
}


void App::_imguiAgentActionQueueWindow()
{
	if (ImGui::Begin(selectable_agent_stats[show_agent_stats_at_index], &show_agent_stats_window))
	{
		ImGui::Text("Action Queue");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Here we show the current queue of actions for the agent");
	}

	ImGui::End();
}






void App::_handleInput()
{
	using namespace std;

	// Do not allow capturing input to imgui and app at same time.
	if (!imgui_has_focus)
	{
		if (GetKey(olc::Key::TAB).bPressed)
		{
			imgui_demo_window = (imgui_demo_window == false) ? true : false;
			gameobjects_window = (gameobjects_window == false) ? true : false;
		}


		if (GetMouse(1).bPressed) tv.StartPan(GetMousePos());
		if (GetMouse(1).bHeld) tv.UpdatePan(GetMousePos());
		if (GetMouse(1).bReleased) tv.EndPan(GetMousePos());
		if (GetMouseWheel() > 0) tv.ZoomAtScreenPos(2.0f, GetMousePos());
		if (GetMouseWheel() < 0) tv.ZoomAtScreenPos(0.5f, GetMousePos());
	}
}



