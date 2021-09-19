#include "main.h"


/*
* IMGUI related variables, for easier iterating.
*/
static bool imgui_demo_window = false;
static bool gameobjects_window = true;
static bool imgui_has_focus = false;
static const char* selectable_agent_stats[] = { "Stats", "Agent Beliefs", "Goals", "Available Actions", "Action Queue", "Agent Schedule"};
static int show_agent_stats_at_index = -1;
//static Agent* agent_showing_stats = nullptr;
static bool show_agent_stats_window = false;
static bool show_nav_mesh = false;
static bool show_nav_path = false;
static bool show_agent_action = false;

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

	AI::get()->executeBehaviorTrees();

	// Update all agents.
	for (auto& go : GameObjectStorage::get()->getStorage())
	{
		// Update Navigator, for each entity which can move, let it move.
		if (go->hasComponent("Navigator"))
		{
			go->getComponent<NavigatorCmp>("Navigator")->update(GameWorldTime::get()->getTimeSpeed());
		}
	}


	// Application rendering.
	olc::vi2d topleft = tv.GetTopLeftTile().max({ 0, 0 });
	olc::vi2d bottomright = tv.GetBottomRightTile().min({ DEFAULT_MAPSIZE_X, DEFAULT_MAPSIZE_Y });
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


	// Draw Buildings
	for (auto& go : GameObjectStorage::get()->getStorage())
	{
		if (go->hasComponent("Renderable"))
		{
			RendererableCmp* render = go->getComponent<RendererableCmp>("Renderable");
			if (render->render)
			{
				if (go->getTag().find("Building") != std::string::npos)
				{
					olc::Pixel color;
					color = _getColorFromString(render->color);

					TransformCmp* transform = go->getComponent<TransformCmp>("Transform");

					// Draw solid building ground
					tv.FillRect(olc::vf2d(transform->xpos, transform->ypos), olc::vf2d(render->width, render->height), color);

					
					// Draw walkable building
					if (go->hasComponent("WalkableBuilding"))
					{
						WalkableBuildingCmp* wb = go->getComponent<WalkableBuildingCmp>("WalkableBuilding");
						std::pair<int, int> door = wb->getDoorToBuilding();

						// Draw Doorway
						tv.FillRect(olc::vf2d(door.first, door.second), olc::vf2d(1, 1), olc::DARK_GREY);

						// Draw building inside
						tv.FillRect(olc::vf2d(transform->xpos + 1, transform->ypos + 1), olc::vf2d(render->width - 2, render->height - 2), olc::VERY_DARK_GREY);
					}
				}
			}
		}
	}


	// Draw Gameobjects, such as NPCs and Furniture
	for (auto& go : GameObjectStorage::get()->getStorage())
	{
		if (go->hasComponent("Renderable"))
		{
			RendererableCmp* render = go->getComponent<RendererableCmp>("Renderable");
			if (render->render)
			{
				if (go->getTag().find("Furniture") != std::string::npos)
				{
					olc::Pixel color;

					TransformCmp* transform = go->getComponent<TransformCmp>("Transform");
					
					color = _getColorFromString(render->color);

					// Draw entity
					tv.FillRect(olc::vf2d(transform->xpos, transform->ypos), olc::vf2d(render->width, render->height), color);
				}
			}
		}
	}

	// Draw NPCs
	for (auto& go : GameObjectStorage::get()->getStorage())
	{
		if (go->hasComponent("Renderable"))
		{
			RendererableCmp* render = go->getComponent<RendererableCmp>("Renderable");
			if (render->render)
			{
				if (go->getTag().find("Agent") != std::string::npos)
				{
					// Draw entity
					olc::Pixel color;

					TransformCmp* transform = go->getComponent<TransformCmp>("Transform");

					color = _getColorFromString(render->color);

					tv.FillRect(olc::vf2d(transform->xpos, transform->ypos), olc::vf2d(render->width, render->height), color);


					// Draw animation if needed
					if (go->hasComponent("Animator"))
					{
						AnimatorCmp* anim = go->getComponent< AnimatorCmp >("Animator");

						AnimatorCmp::Animations currAnim = anim->getAnimation();

						switch (currAnim)
						{
						case AnimatorCmp::Animations::ANIM_SLEEP:
							tv.DrawStringDecal(olc::vf2d(transform->xpos + render->width / 4, transform->ypos - render->height / 2), "Sleep");
							break;

						case AnimatorCmp::Animations::ANIM_WALK:
							tv.DrawStringDecal(olc::vf2d(transform->xpos + render->width / 4, transform->ypos - render->height / 2), "Walk");
							break;

						case AnimatorCmp::Animations::ANIM_IDLE:
							tv.DrawStringDecal(olc::vf2d(transform->xpos + render->width / 4, transform->ypos - render->height / 2), "Idle");
							break;

						case AnimatorCmp::Animations::ANIM_EAT:
							tv.DrawStringDecal(olc::vf2d(transform->xpos + render->width / 4, transform->ypos - render->height / 2), "Eat");
							break;

						case AnimatorCmp::Animations::ANIM_DRINK:
							tv.DrawStringDecal(olc::vf2d(transform->xpos + render->width / 4, transform->ypos - render->height / 2), "Drink");
							break;


						default:
							break;
						}
					}

				}
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


	// Draw objects that are colliding
	for (auto& first : GameObjectStorage::get()->getStorage())
	{
		for (auto& second : GameObjectStorage::get()->getStorage())
		{
			if (first->getTag().compare(second->getTag()) == 0) continue;

			if (first->hasComponent("CollisionBox") && second->hasComponent("CollisionBox"))
			{
				if (first->getComponent<CollisionBoxCmp>("CollisionBox")->resolve(second) &&
					second->getComponent<CollisionBoxCmp>("CollisionBox")->resolve(first))
				{
					TransformCmp* tr = first->getComponent<TransformCmp>("Transform");
					CollisionBoxCmp* c = first->getComponent<CollisionBoxCmp>("CollisionBox");

					tv.DrawRect(olc::vf2d(tr->xpos - 0.1f, tr->ypos - 0.1f), olc::vf2d(c->width + 0.1f, c->height + 0.1f), olc::DARK_RED);
				}
			}
		}

	}

	/*
	for (auto& coll : ComponentStorage::get()->getAllOfType<CollisionBoxCmp>("CollisionBox")) // Update collision detection.
	{
		for (auto& go : GameObjectStorage::get()->getStorage())
		{
			// Do not draw collision with self
			if (coll->this_agent->getTag().compare(go->getTag()) == 0) continue;

			// Resolve collision with another object
			if (coll->resolve(go))
			{
				TransformCmp* tr = static_cast<TransformCmp*>(coll->this_agent->getComponent("Transform"));
				CollisionBoxCmp* c = static_cast<CollisionBoxCmp*>(coll->this_agent->getComponent("CollisionBox"));

				tv.DrawRect(olc::vf2d(tr->xpos - 0.1f, tr->ypos - 0.1f), olc::vf2d(c->width + 0.1f, c->height + 0.1f), olc::DARK_RED);
			}
		}
	}
	*/

	if (show_nav_mesh)
	{
		// Draw Nodes
		std::vector<std::vector<int>> nodes = NavMesh::get()->getNavGraph();
		Graph* graph = NavMesh::get()->getGraph();
		for (int i = 0; i < nodes.size(); i++)
		{
			for (int j = 0; j < nodes[i].size(); j++)
			{
				if (nodes[i][j] == 1)
				{
					tv.DrawCircle(olc::vf2d(i + 0.5f, j + 0.5f), 0.1f, olc::WHITE);
				}
			}
		}

		// Draw Edges
		for (auto& e : graph->edges)
		{
			float a, b, x, y;

			a = edge_from_x(e);
			b = edge_from_y(e);
			x = edge_to_x(e);
			y = edge_to_y(e);

			tv.DrawLine(olc::vf2d(a + 0.5f, b + 0.5f), olc::vf2d(x + 0.5f, y + 0.5f), olc::YELLOW);
		}
	}


	if (show_nav_path)
	{
		// Draw waypoints of selected entity,
		// if it has one.
		if (selected_gameobject)
		{
			if (selected_gameobject->hasComponent("Navigator"))
			{
				NavigatorCmp* nav = selected_gameobject->getComponent<NavigatorCmp>("Navigator");


				for (int i = 0; i < nav->movementPoints.size(); i++)
				{
					if (i == 0)
					{
						// Draw source with special color.
						tv.DrawCircle(olc::vf2d(node_x(nav->movementPoints[i]) + 0.5f, node_y(nav->movementPoints[i]) + 0.5f), 0.1f, olc::RED);
						continue;
					}

					if (i + 1 == nav->movementPoints.size())
					{
						// Draw destination with special color.
						tv.DrawCircle(olc::vf2d(node_x(nav->movementPoints[i]) + 0.5f, node_y(nav->movementPoints[i]) + 0.5f), 0.1f, olc::GREEN);
						continue;
					}

					tv.DrawCircle(olc::vf2d(node_x(nav->movementPoints[i]) + 0.5f, node_y(nav->movementPoints[i]) + 0.5f), 0.1f, olc::MAGENTA);
				}
			}
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



	tv = olc::TileTransformedView({ ScreenWidth(), ScreenHeight() }, { DEFAULT_MAPSIZE_X, DEFAULT_MAPSIZE_Y });

	
	GameWorldTime::get()->setTimeSpeed(0.016);


	GameObjectCreator creator;
	GameObject* fred = creator.create("GOAP/Gameobjects/NPC.xml", "Fred", 25, 25);
	GameObject* tavern = creator.create("GOAP/Gameobjects/Tavern.xml", "Tavern", 10, 5);
	GameObject* shop = creator.create("GOAP/Gameobjects/Shop.xml", "Shop", 20, 5);
	GameObject* freds_house = creator.create("GOAP/Gameobjects/HouseSmall.xml", "Freds_House", 6, 24);


	fred->getComponent<OwnershipCmp>("Ownership")->addGameobject(freds_house);


	// Blackboard and Tree for Agent Fred.
	BTBlackboard* treeBlackboard = new BTBlackboard("TreeBlackboard");
	BTBlackboard* blackboard = new BTBlackboard("Fred_Blackboard");
	BTFactory factory("Fred_Agent_Tree");
	BehaviorTree* tree = nullptr;
	tree = factory.add<BTSequence>("StartingSequence")
									.add<BTUpdateAgentNeeds>("UpdateNeeds", fred)
										.end()
									.add<BTFallback>("NeedsFallback")
										.add<BTSequence>("AgentSleepLogic")
											.add<BTIsAgentSleepy>("IsAgentSleepy", fred)
												.end()
											.add<BTSequence>("AgentGoSleepSequence")
												.add<BTFindAgentObject>("FindAgentBed", fred, "Bed", BTFindAgentObject::SearchPolicy::SP_OWNERSHIP, blackboard, "Bed")
													.end()
												.add<BTGoToAgentObject>("GoToAgentBed", fred, blackboard, "Bed")
													.end()
												.add<BTSetCurrentExecutedNode>("StoreExecutedNode", treeBlackboard)
													.add<BTDiminishAgentNeed>("AgentSleep", fred, "Sleep", 5.0f)
														.end()
													.end()
												.end()
											.end()
										.add<BTMoveToRandomPosition>("RandomMovement", fred)
								.build();
						
	tree->setTreeBlackboard(treeBlackboard);
	AI::get()->addBehaviorTree(tree);


	NavMesh::get()->bake();

	
	return true;
}



int main()
{
	App demo;
	if (demo.Construct(350, 230, 4, 4))
		demo.Start();



	AI::del();
	NavMesh::del();
	GameObjectStorage::del();


	_CrtCheckMemory();
	_CrtDumpMemoryLeaks();

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

			if (ImGui::BeginMenu("Navigator"))
			{

				if (ImGui::MenuItem("NavMesh"))
				{
					show_nav_mesh = (show_nav_mesh == true) ? false : true;
				}
				if (ImGui::MenuItem("MovePoints"))
				{
					show_nav_path = (show_nav_path == true) ? false : true;
				}
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
				/*
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
				*/

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

							if (ImGui::SliderInt2("Position", v, -DEFAULT_MAPSIZE_X, DEFAULT_MAPSIZE_X))
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


						if (cmp->getType().find("AgentStats") != std::string::npos)
						{
							AgentStatsCmp* stats = static_cast<AgentStatsCmp*>(cmp);

						}

						if (cmp->getType().find("AgentNeeds") != std::string::npos)
						{
							AgentNeedsCmp* need = go->getComponent<AgentNeedsCmp>("AgentNeeds");

							ImGui::Text("Sleep: %.3f", need->getNeed("Sleep"));
							ImGui::Text("Hunger: %.3f", need->getNeed("Hunger"));
							ImGui::Text("Thirst: %.3f", need->getNeed("Thirst"));
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
	/*
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
	*/
}


void App::_imguiAgentScheduleWindow()
{
	if (ImGui::Begin(selectable_agent_stats[show_agent_stats_at_index], &show_agent_stats_window))
	{
		ImGui::Text("Role: ");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("The role of an agent defines his daily schedule");

		ImGui::SameLine();
		//ImGui::Text(agent_showing_stats->agentRole.c_str());
		ImGui::Separator();

		/*
		for (auto& schedule_entry : agent_showing_stats->daySchedule->schedule)
		{
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
		*/
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

		/*
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
		*/
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
	
		/*
		for (auto& action : agent_showing_stats->availableActions)
		{

			ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Action: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), action->action_name.c_str());

			ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Cost: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), std::to_string(action->cost).c_str());

			ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Duration: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), std::to_string(action->duration).c_str());

			ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Target: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), action->target_name.c_str());

			ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Running: ");
			ImGui::SameLine();
			std::string running = action->running == true ? "true" : "false";
			ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), running.c_str());

			ImGui::Separator();


			if (action->pre_conditions.size() == 0)
			{
				ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Condition: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), "None");
			}
			else
			{
				for (auto& cond : action->pre_conditions)
				{

					ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Condition: ");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), cond->key.c_str());

					ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Value: ");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), std::to_string(cond->value).c_str());

				}
			}

			if (action->effects.size() == 0)
			{
				ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Effect: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), "None");
			}
			else
			{
				for (auto& eff : action->effects)
				{
					ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Effect: ");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), eff->key.c_str());

					ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "Value: ");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), std::to_string(eff->value).c_str());

				}
			}

			ImGui::Separator();
			ImGui::Separator();
		}
		*/
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



