#pragma once
#include "GuiComponent.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GameStateHandler.h"
#include "UserStatistics.h"


class GameOverGuiComponent : public GuiComponent
{
public:
	GameOverGuiComponent(GameStateHandler* gameStateHandler, UserStatistics* userStatistics);
	~GameOverGuiComponent();

	void draw(GLFWwindow* window) override;
	void update(float deltaTime) override;

	void setMenuActive(bool* value);

private:
	GameStateHandler* gameStateHandler;
	UserStatistics* userStatistics;

	bool* menuActive;

	const int buttonSizeX = 200;
	const int buttonSizeY = 75;

	int windowSizeHeight;
	int windowSizeWidth;
};

