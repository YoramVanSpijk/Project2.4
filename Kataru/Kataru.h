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
#include "UserStatistics.h"
#include "CollisionHandler.h"
#include "ColorDetector.h"
#include "TextWriter.h"
#include "ObjModel.h"
#include "GameOverGuiComponent.h"
#include "CalibrateGuiComponent.h"

class Kataru
{
public:
	Kataru();
	~Kataru();

	void attachGameObject(GameObject* gameObject, Component* component, glm::vec3 pos = glm::vec3(0, 0, 0));
	void attachGuiObject(GuiObject* guiObject, GLFWwindow* window, GuiComponent* guiComponent);
	void attachCalibrationGuiObject(GuiObject* guiObject, GLFWwindow* window, GuiComponent* guiComponent);
	void attachGameOverGuiObject(GuiObject* guiObject, GLFWwindow* window, GuiComponent* guiComponent);

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
	TextWriter* tw;

	CollisionHandler* collisionHandler;
	GameStateHandler* gameStateHandler;
	GameStateHandler::GameState currentGameState;

	UserStatistics* userStatistics;
	ColorDetector* colorDetector;
	VisionCamera* visionCam;

	std::vector<GuiObject*> menuGuiObjects;
	std::vector<GuiObject*> gameOverGuiObjects;
	std::vector<GuiObject*> calibrationGuiObjects;
	std::vector<GameObject*> gameObjects;

	std::unique_ptr<ObjSpawner> spawner;

	bool cursorChangeMenu;
	bool cursorChangeGame;

	bool showCalibrationROI = false;

	const char* glsl_version = "#version 130";
};

