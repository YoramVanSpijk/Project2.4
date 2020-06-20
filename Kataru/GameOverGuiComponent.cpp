#include "GameOverGuiComponent.h"

GameOverGuiComponent::GameOverGuiComponent(GameStateHandler* gameStateHandler, UserStatistics* userStatistics)
{
    this->gameStateHandler = gameStateHandler;
    this->userStatistics = userStatistics;
}

GameOverGuiComponent::~GameOverGuiComponent()
{
}

void GameOverGuiComponent::draw(GLFWwindow* window)
{
    glfwGetFramebufferSize(window, &windowSizeHeight, &windowSizeWidth);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(windowSizeHeight, windowSizeWidth));
    //ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("Kataru", menuActive, ImGuiWindowFlags_NoTitleBar);

    ImGui::SetWindowFontScale(1.4);

    int userScore = userStatistics->GetUserScore();
    const char* userName = userStatistics->GetUserName();

    char* scoreText = new char[255];
    sprintf(scoreText, "Score: %d", userScore);

    ImGui::SetCursorPos(ImVec2((windowSizeHeight / 4) - (buttonSizeX / 2), (windowSizeWidth / 2) - 16));
    ImGui::Text(userName);
    ImGui::SetCursorPos(ImVec2((windowSizeHeight / 4) - (buttonSizeX / 2), (windowSizeWidth / 2)));
    ImGui::Text(scoreText);

    ImGui::SetWindowFontScale(1);

    ImGui::SetCursorPos(ImVec2((windowSizeHeight / 2) - (buttonSizeX / 2), (windowSizeWidth / 2) - (buttonSizeY * 1.5)));
    if (ImGui::Button("Play again", ImVec2(buttonSizeX, buttonSizeY)))
    {
        if (gameStateHandler != nullptr)
            gameStateHandler->SetGamestate(GameStateHandler::GameState::Game);
    }

    ImGui::SetCursorPos(ImVec2((windowSizeHeight / 2) - (buttonSizeX / 2), (windowSizeWidth / 2) - (buttonSizeY * 0.5)));
    if (ImGui::Button("Return to menu", ImVec2(buttonSizeX, buttonSizeY)))
    {
        if (gameStateHandler != nullptr)
            gameStateHandler->SetGamestate(GameStateHandler::GameState::Menu);
    }

    ImGui::SetCursorPos(ImVec2((windowSizeHeight / 2) - (buttonSizeX / 2), (windowSizeWidth / 2) + (buttonSizeY * 0.5)));
    if (ImGui::Button("Quit", ImVec2(buttonSizeX, buttonSizeY)))
    {
        if (gameStateHandler != nullptr)
            gameStateHandler->SetGamestate(GameStateHandler::GameState::Quit);
    }
    ImGui::End();
}

void GameOverGuiComponent::update(float deltaTime)
{

}

void GameOverGuiComponent::setMenuActive(bool* value)
{
    menuActive = value;
}