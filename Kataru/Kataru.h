#pragma once

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
#include "ObjSpawner.h"

class Kataru
{
public:
	Kataru();

	void attachGameObject(GameObject* gameObject, Component* component, glm::vec3 pos = glm::vec3(0, 0, 0));
	void attachGuiObject(GuiObject* guiObject, GLFWwindow* window, GuiComponent* guiComponent);

	void initImGui();
	void setMouseCursorVisibilityMenu();
	void setMouseCursorVisibilityGame();
	void setMouseCursorVisibility(int value);

	void init();
	void update();
	void draw();

private:
	GLFWwindow* window;
	double lastFrameTime;

	GameStateHandler* gameStateHandler;
	GameStateHandler::GameState currentGameState;

	std::vector<GuiObject*> guiObjects;
	std::vector<GameObject*> gameObjects;

	bool cursorChangeMenu;
	bool cursorChangeGame;

	const char* glsl_version = "#version 130";
};

