#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "tigl.h"
#include "GameObject.h"
#include "GuiObject.h"
#include "MenuGuiComponent.h"
#include "GameStateHandler.h"
#include "VisionCamera.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "opencv_world341.lib")
#pragma comment(lib, "opencv_world341d.lib")

GLFWwindow* window;
double lastFrameTime;

GameStateHandler* gameStateHandler;
GameStateHandler::GameState currentGameState;

std::vector<GuiObject*> guiObjects;
std::vector<GameObject*> gameObjects;

const char* glsl_version = "#version 130";
void initImGui();

bool cursorChangeMenu;
bool cursorChangeGame;
void setMouseCursorVisibilityMenu();
void setMouseCursorVisibilityGame();
void setMouseCursorVisibility(int value);

void init();
void update();
void draw();

int main(void)
{	
    if (!glfwInit())
        throw "Could not initialize glwf";

    window = glfwCreateWindow(1400, 800, "Kataru", NULL, NULL);
    gameStateHandler = new GameStateHandler();

    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init();
    initImGui();
    init();

    gameStateHandler->SetGamestate(GameStateHandler::GameState::Menu);

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
    return 0;
}

void attachGameObject(GameObject* gameObject, Component* component, glm::vec3 pos = glm::vec3(0, 0, 0))
{
    GameObject* obj = gameObject == nullptr ? new GameObject() : gameObject;
    obj->position = pos;

    if (component != nullptr)
        obj->addComponent(component);

    gameObjects.push_back(obj);
}

void attachGuiObject(GuiObject* guiObject, GLFWwindow* window, GuiComponent* guiComponent)
{
    GuiObject* obj = guiObject == nullptr ? new GuiObject(window) : guiObject;

    if (guiComponent != nullptr)
        obj->addGuiComponent(guiComponent);

    guiObjects.push_back(obj);
}

void initImGui()
{
    IMGUI_CHECKVERSION();
    ImGuiContext* ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void init()
{
    glEnable(GL_DEPTH_TEST);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });

    cursorChangeMenu = true;
    cursorChangeGame = true;
    lastFrameTime = 0;
    attachGameObject(nullptr, new VisionCamera(window), glm::vec3(0.0f, 0.0f, 0.0f));
    attachGuiObject(nullptr, window, new MenuGuiComponent(gameStateHandler));
}

void setMouseCursorVisibilityMenu()
{
    if (cursorChangeMenu)
    {
        setMouseCursorVisibility(GLFW_CROSSHAIR_CURSOR);
        cursorChangeMenu = false;
    }
}

void setMouseCursorVisibilityGame()
{
    if (cursorChangeGame)
    {
        setMouseCursorVisibility(GLFW_CURSOR_DISABLED);
        cursorChangeGame = false;
    }
}

void setMouseCursorVisibility(int value)
{
    glfwSetInputMode(window, GLFW_CURSOR, value);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void update()
{
    double currentFrameTime = glfwGetTime();
    double deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    gameStateHandler->GetGamestate(&currentGameState);

    switch (currentGameState)
    {
        case GameStateHandler::GameState::Menu:
            setMouseCursorVisibilityMenu();

            for (size_t i = 0; i < guiObjects.size(); i++)
                guiObjects[i]->update(deltaTime);

            break;
        case GameStateHandler::GameState::Game:
            setMouseCursorVisibilityGame();

            for (size_t i = 0; i < gameObjects.size(); i++)
                gameObjects[i]->update(deltaTime);

            break;
        case GameStateHandler::GameState::Quit:
            glfwSetWindowShouldClose(window, true);
            break;
    }
}

void draw()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glm::mat4 projection = glm::perspective(glm::radians(55.0f), width / (float)height, 0.1f, 100.0f);
    tigl::shader->setProjectionMatrix(projection);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (currentGameState)
    {
        case GameStateHandler::GameState::Menu:
            for (size_t i = 0; i < guiObjects.size(); i++)
                guiObjects[i]->draw();

            break;
        case GameStateHandler::GameState::Game:
            for (size_t i = 0; i < gameObjects.size(); i++)
                gameObjects[i]->draw();

            break;
    }
}