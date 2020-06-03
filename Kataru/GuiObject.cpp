#include "GuiObject.h"

#include "GuiComponent.h"

GuiObject::GuiObject(GLFWwindow* window)
{
	this->window = window;
}

GuiObject::~GuiObject()
{
}

void GuiObject::addGuiComponent(GuiComponent* component)
{
	component->setGameObject(this);
	guiComponents.push_back(component);
}

void GuiObject::removeGuiComponent(GuiComponent* component)
{
	guiComponents.erase(std::remove(guiComponents.begin(), guiComponents.end(), component), guiComponents.end());
}

std::vector<GuiComponent*> GuiObject::getGuiComponents()
{
	return std::vector<GuiComponent*>();
}

void GuiObject::update(float deltaTime)
{
	for (size_t i = 0; i < guiComponents.size(); i++)
		guiComponents[i]->update(deltaTime);
}

void GuiObject::draw()
{
	if (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for (size_t i = 0; i < guiComponents.size(); i++)
			guiComponents[i]->draw(window);

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//glfwSwapBuffers(window);
	}
	else
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}