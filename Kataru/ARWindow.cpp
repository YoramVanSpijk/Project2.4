#include "ARWindow.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "tigl.h"

void onDraw(void* param)
{
	ARWindow* ctx = static_cast<ARWindow*>(param);
	if (ctx)
	{
		ctx->draw();

	}

	delete ctx;
}

ARWindow::ARWindow(std::string windowName, cv::Size frameSize) 
	: windowName(windowName), frameSize(frameSize)
{
	cv::namedWindow(windowName, CV_WINDOW_OPENGL);
	cv::resizeWindow(windowName, frameSize.width, frameSize.height);
	cv::setOpenGlContext(windowName);
	cv::setOpenGlDrawCallback(windowName, onDraw, this);

	if (!glfwInit())
	{
		throw "Could not initialize glwf";
	}

	tigl::init();
}

ARWindow::~ARWindow()
{
	cv::setOpenGlDrawCallback(this->windowName, 0, 0);
}

void ARWindow::updateBackground(const cv::Mat& frame)
{
	frame.copyTo(this->background);
}

void ARWindow::updateWindow()
{
	cv::updateWindow(this->windowName);
}

void ARWindow::draw()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	drawBackground();
	draw3DLayer();
	glFlush();
}

void ARWindow::drawBackground()
{
	cv::imshow(this->windowName, this->background);
}

void ARWindow::draw3DLayer()
{
	//draw
}
