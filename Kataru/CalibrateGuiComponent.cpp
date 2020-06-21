#include "CalibrateGuiComponent.h"

CalibrateGuiComponent::CalibrateGuiComponent(GameStateHandler* gameStateHandler, UserStatistics* userStatistics, bool* colorDetected)
{
    this->gameStateHandler = gameStateHandler;
    this->userStatistics = userStatistics;
    this->colorDetected = colorDetected;
}

CalibrateGuiComponent::~CalibrateGuiComponent()
{
    delete this->gameStateHandler;
    delete this->userStatistics;
    delete this->colorDetected;
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

    ImGui::SetCursorPos(ImVec2((windowSizeHeight / 2) - (buttonSizeX / 2), (windowSizeWidth / 8) * 2));
    ImGui::Text("Press 'TAB' to calibrate");

    ImGui::SetCursorPos(ImVec2((windowSizeHeight / 2) - (buttonSizeX / 2), (windowSizeWidth / 8) * 6 - (buttonSizeY / 1.4)));
    ImGui::Text("Enter a catchy name:");

    ImGui::SetCursorPos(ImVec2((windowSizeHeight / 2) - (buttonSizeX / 2), (windowSizeWidth / 8) * 6 - (buttonSizeY / 2)));
    ImGui::SetNextItemWidth(buttonSizeX);

    if (ImGui::InputText(userStatistics->GetUserName(), nameBuffer, sizeof(char[255]), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
    {
        if (strlen(nameBuffer) > 0)
            userStatistics->SetUserName(nameBuffer);
    }

    if (*colorDetected)
    {
        ImGui::SetCursorPos(ImVec2((windowSizeHeight / 2) - (buttonSizeX / 2), (windowSizeWidth / 8) * 6));
        if (ImGui::Button("Continue", ImVec2(buttonSizeX, buttonSizeY)))
        {
            if (gameStateHandler != nullptr)
                gameStateHandler->SetGamestate(GameStateHandler::GameState::Game);
        }
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