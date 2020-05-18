#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "tigl.h"
#include "FpsCam.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "opencv_world341.lib")
#pragma comment(lib, "opencv_world341d.lib")

using namespace cv;

GLFWwindow* window;
GLuint buttonTexId = -2;
GLuint camTexId = -3;

void init();
void update();
void draw();

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(1400, 800, "Hello World", NULL, NULL);
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

FpsCam* camera;
VideoCapture cap;

void init()
{
    glEnable(GL_DEPTH_TEST);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });
    camera = new FpsCam(window);


    // Camera
    VideoCapture c(0);
    cap = c;
    if (!cap.isOpened())
    {
        std::cout << "Cannot open the video cam" << std::endl;
    }

    glGenTextures(1, &camTexId);
    glBindTexture(GL_TEXTURE_2D, camTexId);

    //Buttons
    glGenTextures(1, &buttonTexId);
    glBindTexture(GL_TEXTURE_2D, buttonTexId);

    int width, height, comp;
    unsigned char* data = stbi_load("button_texture_atlas.png", &width, &height, &comp, 4);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Wordt op de heap gezet
    stbi_image_free(data);
}


void update()
{
    camera->update(window);
}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera->getMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    tigl::shader->enableColor(false);
    tigl::shader->enableTexture(true);

    Mat frame;
    if (cap.read(frame)) {
        flip(frame, frame, 3);

        cvtColor(frame, frame, CV_BGR2RGB);
        glBindTexture(GL_TEXTURE_2D, camTexId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    tigl::begin(GL_QUADS);
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0, 0)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec2(1, 0)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(2.0f, 1.0f, 0.0f), glm::vec2(1, -1)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, -1)));
    tigl::end();

    glBindTexture(GL_TEXTURE_2D, buttonTexId);

    glm::mat4 cubeModel = glm::mat4(1.0f);
    cubeModel = glm::translate(cubeModel, glm::vec3(-2.5, 0, 0));
    tigl::shader->setModelMatrix(cubeModel);

    float textSizeX = 1 * 600.0f;

    glm::vec2 texPosition(0, 0);
    int index = 1;
    texPosition.x = index * textSizeX;

    tigl::begin(GL_QUADS);
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(0.0f, 0.0f, 0.0f), texPosition + glm::vec2(0, 0)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(2.0f, 0.0f, 0.0f), texPosition + glm::vec2(1, 0)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(2.0f, 1.0f, 0.0f), texPosition + glm::vec2(1, -1)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(0.0f, 1.0f, 0.0f), texPosition + glm::vec2(0, -1)));
    tigl::end();
}
