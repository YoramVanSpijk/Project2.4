#pragma once
#include "GuiComponent.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GameStateHandler.h"

class CalibrateGuiComponent : public GuiComponent
{
public:
	CalibrateGuiComponent(GameStateHandler* gameStateHandler);
	~CalibrateGuiComponent();

	void draw(GLFWwindow* window) override;
	void update(float deltaTime) override;

	void setMenuActive(bool* value);

private:
	GameStateHandler* gameStateHandler;
	bool* menuActive;

	const int buttonSizeX = 200;
	const int buttonSizeY = 75;

	int windowSizeHeight;
	int windowSizeWidth;
};

