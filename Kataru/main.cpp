#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "tigl.h"
#include "FpsCam.h"
#include "GameObject.h"
#include "CameraObject.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "opencv_world341.lib")
#pragma comment(lib, "opencv_world341d.lib")

GLFWwindow* window;
double lastFrameTime;

std::vector<CameraObject*> cameraObjects;
std::vector<GameObject*> gameObjects;

void init();
void update();
void draw();

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";

    window = glfwCreateWindow(1400, 800, "Kataru", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init();
    init();

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

void attachGameObject(GameObject* gameObject = nullptr, Component* component = nullptr, glm::vec3 pos = glm::vec3(0, 0, 0))
{
    GameObject* obj = gameObject == nullptr ? new GameObject() : gameObject;
    obj->position = pos;

    if (component != nullptr)
        obj->addComponent(component);

    gameObjects.push_back(obj);
}

void attachCameraObject(GLFWwindow* window, CameraObject* cameraObject = nullptr, FpsCam* camera = nullptr)
{
    CameraObject* obj = cameraObject == nullptr ? new CameraObject(window) : cameraObject;

    if (camera != nullptr)
        obj->addCamera(camera);

    cameraObjects.push_back(obj);
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });
    
    lastFrameTime = 0;
    attachCameraObject(window, nullptr, new FpsCam(window));
}


void update()
{
    double currentFrameTime = glfwGetTime();
    double deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;
    
    for (size_t i = 0; i < cameraObjects.size(); i++)
        cameraObjects[i]->update(window);

    for (size_t i = 0; i < gameObjects.size(); i++)
        gameObjects[i]->update(deltaTime);
}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (size_t i = 0; i < cameraObjects.size(); i++)
        cameraObjects[i]->draw();

    for (size_t i = 0; i < gameObjects.size(); i++)
        gameObjects[i]->draw();
}