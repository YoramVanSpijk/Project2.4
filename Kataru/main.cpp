#include <iostream>
#include <chrono>
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "tigl.h"
#include "ARWindow.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "opencv_world341.lib")
#pragma comment(lib, "opencv_world341d.lib")

GLFWwindow* window;

void processVideo(cv::VideoCapture& capture);
bool processFrame(const cv::Mat& cameraFrame, ARWindow& window);

int width, height;
int zoom, horizontal;
glm::mat4 view;


int main(void)
{
    std::cout << cv::getBuildInformation() << std::endl;

    cv::VideoCapture cap(0);

    cv::Mat currentFrame;

    if (!cap.isOpened())
    {
        std::cout << "Cannot open the video cam" << std::endl;
        return -1;
    }

    processVideo(cap);

    return 0;
}

void processVideo(cv::VideoCapture& capture)
{
    cv::Mat currentFrame;
    capture >> currentFrame;

    if (currentFrame.empty())
    {
        std::cout << "Cannot read the video stream" << std::endl;
        return;
    }

    cv::Size frameSize(currentFrame.cols, currentFrame.rows);

    ARWindow window("Kataru", frameSize);

    bool shouldQuit = false;
    do
    {
        capture >> currentFrame;
        if (currentFrame.empty())
        {
            shouldQuit = true;
            continue;
        }

        shouldQuit = processFrame(currentFrame, window);
    } while (!shouldQuit);
}

bool processFrame(const cv::Mat& cameraFrame, ARWindow& window)
{
    cv::Mat img = cameraFrame.clone();

    window.updateBackground(img);
    window.updateWindow();

    int keyCode = cv::waitKey(5);

    bool shouldQuit = false;
    if (keyCode == 27 || keyCode == 'q')
    {
        shouldQuit = true;
    }

    return shouldQuit;
}














//void init()
//{
//    zoom = 10;
//    horizontal = 10;
//    glfwGetWindowSize(window, &width, &height);
//
//    glEnable(GL_DEPTH_TEST);
//    tigl::shader->enableColor(true);
//
//    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
//    {
//        if (key == GLFW_KEY_ESCAPE)
//            glfwSetWindowShouldClose(window, true);
//    });
//}
//
//void update()
//{
//
//}
//
//void draw()
//{
//    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glm::mat4 projection = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 100.0f);
//    view = glm::lookAt(glm::vec3(horizontal, 5, zoom), glm::vec3(horizontal, 0, 0), glm::vec3(0, 1, 0));
//    tigl::shader->setProjectionMatrix(projection);
//    tigl::shader->setViewMatrix(view);
//
//}
