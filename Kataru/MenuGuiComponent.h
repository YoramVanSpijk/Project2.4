#pragma once
#include "GuiComponent.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GuiObject.h"

class MenuGuiComponent : public GuiComponent
{
public:
	void draw(GLFWwindow* window);
	void update(float deltaTime);
};

