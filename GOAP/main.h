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
#include "GOAPInterface.h"



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

	olc::imgui::PGE_ImGUI pge_imgui;
	int m_GameLayer;


private:

	void _onImGui();
	void _handleInput();
};