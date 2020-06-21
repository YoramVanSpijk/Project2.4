#include "CalibrateGuiComponent.h"

CalibrateGuiComponent::CalibrateGuiComponent(GameStateHandler* gameStateHandler)
{
    this->gameStateHandler = gameStateHandler;
}

CalibrateGuiComponent::~CalibrateGuiComponent()
{
}

void CalibrateGuiComponent::draw(GLFWwindow* window)
{
    glfwGetFramebufferSize(window, &windowSizeHeight, &windowSizeWidth);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(windowSizeHeight, windowSizeWidth));

    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoBackground;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;
    windowFlags |= ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("Calibration", menuActive, windowFlags);
    ImGui::SetCursorPos(ImVec2((windowSizeHeight / 2) - (buttonSizeX / 2), (windowSizeWidth / 8) * 6));
    if (ImGui::Button("Continue", ImVec2(buttonSizeX, buttonSizeY)))
    {
        if (gameStateHandler != nullptr)
            gameStateHandler->SetGamestate(GameStateHandler::GameState::Game);
    }
    ImGui::End();
}

void CalibrateGuiComponent::update(float deltaTime)
{

}

void CalibrateGuiComponent::setMenuActive(bool* value)
{
    menuActive = value;
}