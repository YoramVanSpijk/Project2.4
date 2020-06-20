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
#pragma comment(lib, "opencv_world341d.lib")

GLFWwindow* window;
double lastFrameTime;

GameStateHandler* gameStateHandler;
GameStateHandler::GameState currentGameState;

std::vector<GuiObject*> guiObjects;
std::vector<GameObject*> gameObjects;
VisionCamera* vc;

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
    attachGameObject(nullptr, vc = new VisionCamera(window), glm::vec3(0.0f, 0.0f, 0.0f));
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

void drawCircleHitbox(float cx, float cy, float r)
{
    auto const segments = 12;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2.0f);

    auto hitboxModel = glm::mat4(1.0f);
    hitboxModel = glm::scale(hitboxModel, glm::vec3(0.05f, 0.05f, 1.0f));
    hitboxModel = glm::translate(hitboxModel, glm::vec3(cx, cy, -1.0f));
    tigl::shader->setModelMatrix(hitboxModel);
    tigl::shader->enableTexture(false);
    tigl::shader->enableColorMult(true);

    tigl::begin(GL_LINE_LOOP);
    for (int ii = 0; ii < segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(segments); // get the current angle 

        float x = r * cosf(theta); // calculate the x component 
        float y = r * sinf(theta); // calculate the y component 

        tigl::shader->setColorMult(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)); // Orange
        tigl::addVertex(tigl::Vertex::P(glm::vec3(x + cx, y + cy, 0.0f)));
    }
    tigl::end();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    tigl::shader->enableColorMult(false);
}

struct Circle
{
    float r;
    float x;
    float y;
};

float xCoordH = 0.0f;
float yCoordH = 0.0f;

void draw()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glm::mat4 projection = glm::perspective(glm::radians(55.0f), width / (float)height, 0.1f, 100.0f);
    tigl::shader->setProjectionMatrix(projection);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Move object
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        xCoordH -= 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        xCoordH += 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        yCoordH += 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        yCoordH -= 0.1f;
    }

    switch (currentGameState)
    {
        case GameStateHandler::GameState::Menu:
            for (size_t i = 0; i < guiObjects.size(); i++)
                guiObjects[i]->draw();

            break;
        case GameStateHandler::GameState::Game:

            // Get current point
            cv::Point2f handCoord(vc->getCurrentPoint());
            std::cout << "hX: " << handCoord.x << ", hY: " << handCoord.y << "\n";

            // struct hitboxes for objects
            struct Circle handHitBox, objectHitBox;
            handHitBox.r = 15.0f;
            handHitBox.x = handCoord.x;
            handHitBox.y = handCoord.y;

            objectHitBox.r = 15.0f;
            objectHitBox.x = (xCoordH + 0.15f) * 25;
            objectHitBox.y = yCoordH * (200 / 6);

            std::cout << "oX: " << objectHitBox.x << ", oY: " << objectHitBox.y << "\n";

            // Calculate distance between hitboxes
            auto dx = handHitBox.x - objectHitBox.x;
            auto dy = handHitBox.y - objectHitBox.y;
            auto distance = sqrt(dx * dx + dy * dy);

            // Check collision
            if (distance < handHitBox.r + objectHitBox.r) {
                std::cout << "COL\n";
            }

            // Draw hitbox
            drawCircleHitbox((xCoordH + 0.15f), yCoordH, objectHitBox.r / 10);

            // Draw objects
            for (size_t i = 0; i < gameObjects.size(); i++)
                gameObjects[i]->draw();

            break;
    }
}