#include "main.h"
#include "GameObject.h"


void App::DrawUI(void)
{
	//This finishes the Dear ImGui and renders it to the screen
	pge_imgui.ImGui_ImplPGE_Render();
}


bool App::OnUserUpdate(float fElapsedTime)
{



	// For Rendering IMGUI.
	SetDrawTarget((uint8_t)m_GameLayer);
	ImGui::ShowDemoWindow();
	
	
	return true;
}


bool App::OnUserCreate()
{
	m_GameLayer = CreateLayer();
	EnableLayer(m_GameLayer, true);
	SetLayerCustomRenderFunction(0, std::bind(&App::DrawUI, this));


	for (int i = 0; i < 100; i++)
	{
		new GameObject("GameObject_" + std::to_string(i));
	}


	for (auto& go: GameObjectStorage::get()->getStorage())
	{
		using namespace std;
		cout << "tag: " << go->tag << "  hash: " << go->hash << endl;
	}

	GameObjectStorage::del();

	return false;
}



int main()
{
	App demo;
	if (demo.Construct(640, 360, 2, 2))
		demo.Start();
	return 0;
}