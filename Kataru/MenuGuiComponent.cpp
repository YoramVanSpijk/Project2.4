#include "MenuGuiComponent.h"

void MenuGuiComponent::draw(GLFWwindow* window)
{
    ImGui::Text("Hello, world %d", 123);

    //char* buff;
    //ImGui::InputText("string", buff, IM_ARRAYSIZE(buff));

    float f;
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
}

void MenuGuiComponent::update(float deltaTime)
{
}