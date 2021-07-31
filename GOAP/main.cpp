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
	_handleInput();



	// Application rendering.
	for (int x = 0; x < ScreenWidth(); x++)
		for (int y = 0; y < ScreenHeight(); y++)
			Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));




	// For Rendering IMGUI.
	_onImGui();
	
	return true;
}


bool App::OnUserCreate()
{
	m_GameLayer = CreateLayer();
	EnableLayer(m_GameLayer, true);
	SetLayerCustomRenderFunction(0, std::bind(&App::DrawUI, this));



	for (int i = 0; i < 50; i++)
	{
		GameObject* g = new GameObject("GameObject_" + std::to_string(i));
		g->AddComponent(new TransformCmp("Transform"));
	}



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
						if (cmp->type.compare("Transform") == 0)
						{
							float v[2];
							v[0] = static_cast<TransformCmp*>(cmp)->xpos;
							v[1] = static_cast<TransformCmp*>(cmp)->ypos;

							ImGui::SliderFloat2("Position", v, -100.0f, 100.0f, "%.1f", 1.0f);

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

}