#include "main.h"
#include "GameObject.h"


/*
* IMGUI related variables, for easier iterating.
*/
static bool imgui_demo_window = false;
static bool gameobjects_window = true;
static bool imgui_has_focus = false;

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
	Clear(olc::BLACK);

	_handleInput();



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


	//Agent* npc = new Agent("Innkeeper_Walter"); // Create agent.
	//npc->init("TODO.json"); // Get all available action for agent.
	//npc->awake(); // Initialize to work in the engine.


	//tv.FillRectDecal(olc::vi2d(1, 1), olc::vi2d(4, 3), olc::GREY);

	//tv.FillRectDecal(olc::vi2d(5, 3), olc::vi2d(8, 8), olc::GREY);

	//tv.FillRectDecal(olc::vf2d(1, 1), olc::vf2d(1, 1), olc::DARK_GREEN);

	//tv.FillRectDecal(olc::vf2d(2, 1), olc::vf2d(1, 1), olc::DARK_RED);





	for (auto& go : GameObjectStorage::get()->getStorage())
	{
		olc::Pixel color;

		// Just to get color string and to render it then we need O(n*n), which is baad.
		if (go->hasComponent("Renderable"))
		{
			TransformCmp* tr = static_cast<TransformCmp*> (go->getComponent("Transform"));
			RendererableCmp* rc = static_cast<RendererableCmp*> (go->getComponent("Renderable"));


			if (rc->color.compare("grey") == 0) color = olc::GREY;
			else if (rc->color.compare("dark_grey") == 0) color = olc::DARK_GREY;
			else if (rc->color.compare("green") == 0) color = olc::DARK_GREEN;


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

		olc::vf2d p = { tr->xpos + rc->width / 2.0f - 0.3f, tr->ypos + rc->height / 2.0f};
		tv.DrawStringDecal(p, selected_gameobject->tag, olc::RED, olc::vf2d(0.5f, 0.5f));
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





	GameObjectCreator creator;

	GameObject* tavern = creator.create("GOAP/Gameobjects/Tavern.json", "Village Tavern", 3, 5);
	GameObject* shop = creator.create("GOAP/Gameobjects/Shop.json", "Filthy Shop", 13, 5);
	GameObject* house = creator.create("GOAP/Gameobjects/House.json", "Johns House", 21, 2);
	house = creator.create("GOAP/Gameobjects/House.json", "Asmons House", 25, 3);
	house = creator.create("GOAP/Gameobjects/House.json", "Walters House", 28, 8);
	house = creator.create("GOAP/Gameobjects/House.json", "Marthas House", 22, 16);


	GameObject* npc = creator.create("GOAP/Gameobjects/Innkeeper.json", "Innkeeper Simon", 0, 0);

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

	selected_gameobject = nullptr;

	SetDrawTarget((uint8_t)m_GameLayer);

	// CHECK WHETHER IMGUI IS FOCUSED
	if (ImGui::IsAnyItemFocused() || ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive() || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	{
		imgui_has_focus = true;
	}
	else
	{
		imgui_has_focus = false;
	}


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
			/*
			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("GameObject"))
				{
					creating_new_gameobject = true;
				}
				ImGui::EndMenu();
			}
			*/

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	// GAMEOBJECTS WINDOW
	int go_count = GameObjectStorage::get()->getStorage().size();
	ImGui::SetNextWindowPos(ImVec2(1.0f, 1.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(500.0f, 250.0f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin(std::string("GameObjects #" + std::to_string(go_count)).c_str(), &gameobjects_window))
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
				// If GO is an NPC, show ownership and inventory.
				if(go->getTag().find("NPC") != std::string::npos)
				{
					Agent* npc = static_cast<Agent*>(go);

					if (ImGui::TreeNode("Ownership"))
					{
						for (auto& item : npc->agentOwnedObjects)
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