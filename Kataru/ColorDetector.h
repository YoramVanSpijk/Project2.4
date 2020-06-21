#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class ColorDetector
{
public:
	ColorDetector(GLFWwindow* window);
	~ColorDetector();

	cv::Point2f getCurrentPoint();
	void loop(cv::Mat frame);
private:
	//GLFW
	GLFWwindow* window;

	//OpenCV
	const std::string CONTROLS_TAG = "Controls";
	const std::string MASK_TAG = "Mask";

	cv::Mat hsvImage, erosionDst, dilationDst, element, croppedImageL, croppedImageR, lHSV, rHSV, mask;
	int hLowThreshold, hHighThreshold, sLowThreshold, sHighThreshold, vLowThreshold, vHighThreshold, minAreaBlob, maxAreaBlob, offsetHighThreshold, offsetLowThreshold;

	cv::SimpleBlobDetector::Params params;
	cv::Point2f currentPoint;

	//OpenGL
	GLuint camTexId = 99;

	//Others
	bool colorDetected = false;

	void drawROI(cv::Mat frame);
	void drawTexPane();
	void bindTexture(cv::Mat frame);
	void calculateTresholds();
	void createSettingsWindow();
	void applyTransformations(cv::Mat frame);
	void calculateNewPoint(std::vector<cv::KeyPoint> keypoints, int i, cv::Mat frame);
};


