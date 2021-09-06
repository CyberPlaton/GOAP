#pragma once

/*
* OpenGL, GLFW
*/
#include"common/include/GL/glew.h"
#pragma comment(lib, "glew32.lib")

#include"common/include/GLFW/glfw3.h"
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")


/*
* OLC
*/
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"
#define OLC_PGEX_DEAR_IMGUI_IMPLEMENTATION
#include "common/include/imgui_impl_pge.h"


/*
* JSON
*/
#include "common/include/nlohmann/json.hpp"


/*
* GOAP
*/
#include "ComponentSystem.h"
#include "GameObject.h"
#include "Components.h"
#include "GameWorldTime.h"


//#include "GOAPInterface.h"
//#include "GameObjectCreator.h"

#include "BTInterface.h"
#include "BTMoveToRandomPosition.h"


class App : public olc::PixelGameEngine
{
public:
	App() : pge_imgui(false)
	{
		sAppName = "APP";
	}
public:

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	void DrawUI(void);


private:

	olc::TileTransformedView tv;
	olc::imgui::PGE_ImGUI pge_imgui;
	int m_GameLayer;

	BehaviorTree* dude_tree = nullptr;

private:

	void _onImGui();
	void _handleInput();



	void _imguiAgentStatsWindow();
	void _imguiAgentBeliefsWindow();
	void _imguiAgentGoalsWindow();
	void _imguiAgentAvailableActionsWindow();
	void _imguiAgentActionQueueWindow();
	void _imguiAgentScheduleWindow();


	olc::Pixel _getColorFromString(const std::string& c)
	{
		if (c.compare("grey") == 0) return olc::GREY;
		else if (c.compare("dark_grey") == 0)return olc::DARK_GREY;

		else if (c.compare("green") == 0) return olc::GREEN;
		else if (c.compare("dark_green") == 0) return olc::DARK_GREEN;

		else if (c.compare("blue") == 0) return olc::BLUE;
		else if (c.compare("dark_blue") == 0) return olc::DARK_BLUE;

		else if (c.compare("red") == 0) return olc::RED;
		else if (c.compare("dark_red") == 0) return olc::DARK_RED;

		else if (c.compare("yellow") == 0) return olc::YELLOW;
		else if (c.compare("dark_yellow") == 0) return olc::DARK_YELLOW;

		else if (c.compare("magenta") == 0) return olc::MAGENTA;
		else if (c.compare("dark_magenta") == 0) return olc::DARK_MAGENTA;
		
		else return olc::WHITE;
	}
};