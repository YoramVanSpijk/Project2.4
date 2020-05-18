#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <GLFW/glfw3.h>

class ARWindow
{
public:
	ARWindow(std::string name, int width, int height);
	ARWindow(const ARWindow& window);
	~ARWindow();

	void updateWindow();
	void updateBackground();
	void update3D();

	void drawWindow();
	void drawBackground();
	void draw3D();

	void loop();

private:
	std::string name;
	int width;
	int height;
	GLFWwindow* window;
	cv::VideoCapture capture;
};

