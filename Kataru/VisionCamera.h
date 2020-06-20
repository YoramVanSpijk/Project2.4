#pragma once
#include "DrawComponent.h"
#include "tigl.h"

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

	cv::Mat frame, hsvImage, erosionDst, dilationDst, element, croppedImageL, croppedImageR, lHSV, rHSV, mask;
	int hLowThreshold, hHighThreshold, sLowThreshold, sHighThreshold, vLowThreshold, vHighThreshold, minAreaBlob, maxAreaBlob, offsetHighThreshold, offsetLowThreshold;
	
	cv::SimpleBlobDetector::Params params;
	cv::VideoCapture cap;
	cv::Point2f currentPoint;

	//OpenGL
	GLuint camTexId = 99;

	//Others
	bool colorDetected = false;
	bool noCameraError = true;

	void update(float deltaTime);
	void draw();
	void drawROI();
	void drawTexPane();
	void bindTexture();
	void calculateTresholds();
	void createSettingsWindow();
	void applyTransformations();
	void calculateNewPoint(std::vector<cv::KeyPoint> keypoints, int i);
};


