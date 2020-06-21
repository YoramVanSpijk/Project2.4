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
#include "TextWriter.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "opencv_world341.lib")
#pragma comment(lib, "opencv_world341d.lib")

GLFWwindow* window;
//GLuint texId;
double lastFrameTime;
//stbtt_bakedchar cdata[96];

TextWriter* tw;

std::vector<CameraObject*> cameraObjects;
std::vector<GameObject*> gameObjects;

void init();
void update();
void draw();
//void initTrueType();
//void drawText(float, float, std::string);

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
    /*initTrueType();*/
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

    tw = TextWriter::getInstance();
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

    /*drawText(0, 0, "Test");
    drawText(150, 0, "Test iets meer naar rechts");*/

    tw->writeText({0, 0, 0}, "Test");

    for (size_t i = 0; i < cameraObjects.size(); i++)
        cameraObjects[i]->draw();

    for (size_t i = 0; i < gameObjects.size(); i++)
        gameObjects[i]->draw();
}

//void initTrueType() {
//    unsigned char* ttf_buffer = new unsigned char[1 << 20];
//    unsigned char* temp_bitmap = new unsigned char[512 * 512];
//    fread(ttf_buffer, 1, 1 << 20, fopen("c:/windows/fonts/times.ttf", "rb"));
//    stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata);
//    glGenTextures(1, &texId);
//    glBindTexture(GL_TEXTURE_2D, texId);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//    delete[] ttf_buffer;
//    delete[] temp_bitmap;
//}

//void drawText(float x, float y, std::string text) {
//    /*int viewport[4];
//    glGetIntegerv(GL_VIEWPORT, viewport);
//    tigl::shader->setProjectionMatrix(glm::ortho(0.0f, (float)viewport[2], (float)viewport[3], 0.0f, -100.0f, 100.0f));
//    tigl::shader->setViewMatrix(glm::mat4(1.0f));
//    tigl::shader->setModelMatrix(glm::mat4(1.0f));
//
//    tigl::shader->enableColor(false);
//    tigl::shader->enableLighting(false);
//    tigl::shader->enableTexture(true);
//    tigl::shader->enableColorMult(false);*/
//
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glDisable(GL_DEPTH_TEST);
//    glBindTexture(GL_TEXTURE_2D, texId);
//
//    stbtt_aligned_quad q;
//    tigl::begin(GL_QUADS);
//    for (int i = 0; i < text.size(); i++) {
//        if (text[i] >= 32 && text[i] < 128) {
//            stbtt_GetBakedQuad(cdata, 512, 512, text[i] - 32, &x, &y, &q, 1);
//            tigl::addVertex(tigl::Vertex::PT(glm::vec3(q.x0, q.y0, 0), glm::vec2(q.s0, q.t0)));
//            tigl::addVertex(tigl::Vertex::PT(glm::vec3(q.x1, q.y0, 0), glm::vec2(q.s1, q.t0)));
//            tigl::addVertex(tigl::Vertex::PT(glm::vec3(q.x1, q.y1, 0), glm::vec2(q.s1, q.t1)));
//            tigl::addVertex(tigl::Vertex::PT(glm::vec3(q.x0, q.y1, 0), glm::vec2(q.s0, q.t1)));
//        }
//    }
//    tigl::end();
//
//}