#pragma once
#include "DrawComponent.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>

class VisionCamera : public DrawComponent
{
public:
	VisionCamera();
	~VisionCamera();

	bool getCameraError();
	cv::Mat getFrame();
private:
	cv::VideoCapture cap;
	cv::Mat frame;
	bool noCameraError = true;

	void update(float deltaTime);
	void draw();
};


