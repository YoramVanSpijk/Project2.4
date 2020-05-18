#include "ARWindow.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "tigl.h"
#include "FpsCam.h"

FpsCam* camera;
GLuint textureIdCamera = -3;

ARWindow::ARWindow(std::string name, int width, int height)
	: name(name), width(width), height(height)
{
    this->window = glfwCreateWindow(1400, 800, name.c_str(), NULL, NULL);
    if (!this->window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }

    glfwMakeContextCurrent(this->window);

    tigl::init();

    //key callback
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window, true);
        });
    
    camera = new FpsCam(window);
    glEnable(GL_DEPTH_TEST);

    this->capture = cv::VideoCapture(0);
    if (!this->capture.isOpened())
    {
        std::cout << "Cannot open the video cam" << std::endl;
    }

    glGenTextures(1, &textureIdCamera);
    glBindTexture(GL_TEXTURE_2D, textureIdCamera);
}

ARWindow::ARWindow(const ARWindow& window)
{
}

ARWindow::~ARWindow()
{
    glfwTerminate();
}



void ARWindow::updateWindow()
{
    camera->update(this->window);
    this->updateBackground();
    this->update3D();
}

void ARWindow::updateBackground()
{
}

void ARWindow::update3D()
{
}



void ARWindow::drawWindow()
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

    this->drawBackground();
    this->draw3D();
}

void ARWindow::drawBackground()
{
    cv::Mat frame;
    if (this->capture.read(frame)) {
        flip(frame, frame, 3);

        cvtColor(frame, frame, CV_BGR2RGB);
        glBindTexture(GL_TEXTURE_2D, textureIdCamera);
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
}

void ARWindow::draw3D()
{
}

void ARWindow::loop()
{
    while (!glfwWindowShouldClose(this->window))
    {
        this->updateWindow();
        this->drawWindow();
        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }
}

