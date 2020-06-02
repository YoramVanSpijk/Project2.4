#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class GuiComponent;

class GuiObject
{
	std::vector<GuiComponent*> guiComponents;

public:
	GuiObject(GLFWwindow* window);
	~GuiObject();

	void addGuiComponent(GuiComponent* component);
	void removeGuiComponent(GuiComponent* component);
	std::vector<GuiComponent*> getGuiComponents();

	void update(float deltaTime);
	void draw();

private:
	GLFWwindow* window;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};