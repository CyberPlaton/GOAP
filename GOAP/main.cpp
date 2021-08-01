#include "main.h"
#include "GameObject.h"


/*
* IMGUI related variables, for easier iterating.
*/
static bool imgui_demo_window = false;
static bool gameobjects_window = true;




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
			tv.DrawLine(tile, tile + olc::vf2d(0.0f, 1.0f), olc::WHITE);
			tv.DrawLine(tile, tile + olc::vf2d(1.0f, 0.0f), olc::WHITE);
		}
	}


	//Agent* npc = new Agent("Innkeeper_Walter"); // Create agent.
	//npc->init("TODO.json"); // Get all available action for agent.
	//npc->awake(); // Initialize to work in the engine.


	//tv.FillRectDecal(olc::vi2d(1, 1), olc::vi2d(4, 3), olc::GREY);

	//tv.FillRectDecal(olc::vi2d(5, 3), olc::vi2d(8, 8), olc::GREY);

	//tv.FillRectDecal(olc::vf2d(1, 1), olc::vf2d(1, 1), olc::DARK_GREEN);

	//tv.FillRectDecal(olc::vf2d(2, 1), olc::vf2d(1, 1), olc::DARK_RED);



	// For Rendering IMGUI.
	_onImGui();
	
	return true;
}


bool App::OnUserCreate()
{
	m_GameLayer = CreateLayer();
	EnableLayer(m_GameLayer, true);
	SetLayerCustomRenderFunction(0, std::bind(&App::DrawUI, this));


	tv = olc::TileTransformedView({ ScreenWidth(), ScreenHeight() }, {32, 32});





	GameObjectCreator creator;

	GameObject* tavern = creator.create("GOAP/Gameobjects/Tavern.json");



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
			if (ImGui::CollapsingHeader(go->tag.c_str()))
			{
				// Show CMPs
				for (auto& cmp : go->components)
				{
					if (ImGui::TreeNode(cmp->name.c_str()))
					{
						if (cmp->type.find("Transform") == 0)
						{
							int v[2];
							v[0] = static_cast<TransformCmp*>(cmp)->xpos;
							v[1] = static_cast<TransformCmp*>(cmp)->ypos;

							ImGui::SliderInt2("Position", v, -100, 100);

							static_cast<TransformCmp*>(cmp)->xpos = v[0];
							static_cast<TransformCmp*>(cmp)->ypos = v[1];
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