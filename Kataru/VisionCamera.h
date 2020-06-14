#pragma once
#include "DrawComponent.h"
#include "tigl.h"
#include <atomic>
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class VisionCamera : public DrawComponent
{
public:
	VisionCamera(GLFWwindow* window);
	~VisionCamera();

	bool getCameraError();
	cv::Point2f getCurrentPoint();
private:
	//GLFW
	GLFWwindow* window;
	
	//OpenCV
	const std::string CONTROLS_TAG = "Controls";
	const std::string MASK_TAG = "Mask";

	cv::Mat hsvImage, erosionDst, dilationDst, element, croppedImageL, croppedImageR, lHSV, rHSV, mask;
	std::atomic<cv::Mat> frameAtomic;
	
	int hLowThreshold, hHighThreshold, sLowThreshold, sHighThreshold, vLowThreshold, vHighThreshold, minAreaBlob, maxAreaBlob, offsetHighThreshold, offsetLowThreshold;

	cv::SimpleBlobDetector::Params params;
	cv::VideoCapture cap;
	cv::Point2f currentPoint;

	//OpenGL
	GLuint camTexId = 99;

	//Others
	bool colorDetected = false;
	bool noCameraError = true;
	std::thread captureThread;

	void update(float deltaTime);
	void draw();
	void getFrame();
};


