#pragma once
#include "GuiComponent.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GameStateHandler.h"
#include "UserStatistics.h"

class CalibrateGuiComponent : public GuiComponent
{
public:
	CalibrateGuiComponent(GameStateHandler* gameStateHandler, UserStatistics* userStatistics, bool* colorDetected);
	~CalibrateGuiComponent();

	void draw(GLFWwindow* window) override;
	void update(float deltaTime) override;

	void setMenuActive(bool* value);

private:
	GameStateHandler* gameStateHandler;
	UserStatistics* userStatistics;
	bool* colorDetected;

	bool* menuActive;
	char nameBuffer[255] = {};

	const int buttonSizeX = 200;
	const int buttonSizeY = 75;

	int windowSizeHeight;
	int windowSizeWidth;
};

