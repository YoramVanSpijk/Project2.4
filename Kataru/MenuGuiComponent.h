#pragma once
#include "GuiComponent.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GameStateHandler.h"

class MenuGuiComponent : public GuiComponent
{
public:
	MenuGuiComponent(GameStateHandler* gameStateHandler);
	~MenuGuiComponent();

	void draw(GLFWwindow* window);
	void update(float deltaTime);

	void setMenuActive(bool* value);

private:
	GameStateHandler* gameStateHandler;
	bool* menuActive;

	int buttonSizeX = 200;
	int buttonSizeY = 75;

	int windowSizeHeight;
	int windowSizeWidth;
};

