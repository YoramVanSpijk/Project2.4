#include "MenuGuiComponent.h"

MenuGuiComponent::MenuGuiComponent(GameStateHandler* gameStateHandler)
{
    this->gameStateHandler = gameStateHandler;
}

MenuGuiComponent::~MenuGuiComponent()
{
}

void MenuGuiComponent::draw(GLFWwindow* window)
{
    glfwGetFramebufferSize(window, &windowSizeHeight, &windowSizeWidth);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(windowSizeHeight, windowSizeWidth));
    //ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("Kataru", menuActive, ImGuiWindowFlags_NoTitleBar);

    ImGui::SetCursorPos(ImVec2((windowSizeHeight / 2) - (buttonSizeX / 2), (windowSizeWidth / 2) - (buttonSizeY / 2)));
    if (ImGui::Button("Play", ImVec2(buttonSizeX, buttonSizeY)))
    {
        if (gameStateHandler != nullptr)
            gameStateHandler->SetGamestate(GameStateHandler::GameState::Game);
    }

    ImGui::SetCursorPos(ImVec2((windowSizeHeight / 2) - (buttonSizeX / 2), (windowSizeWidth / 2) + (buttonSizeY / 2)));
    if (ImGui::Button("Quit", ImVec2(buttonSizeX, buttonSizeY)))
    {
        if (gameStateHandler != nullptr)
            gameStateHandler->SetGamestate(GameStateHandler::GameState::Quit);
    }
    ImGui::End();
}

void MenuGuiComponent::update(float deltaTime)
{

}

void MenuGuiComponent::setMenuActive(bool* value)
{
    menuActive = value;
}