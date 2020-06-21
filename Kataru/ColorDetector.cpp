#include "ColorDetector.h"
#include "tigl.h"
#include <glm\ext\matrix_transform.hpp>

ColorDetector::ColorDetector(GLFWwindow* window)
{
	this->window = window;
	this->currentPoint = cv::Point2f(0, 0);

    glGenTextures(1, &camTexId);
    glBindTexture(GL_TEXTURE_2D, camTexId);

    this->element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));
    this->offsetLowThreshold = this->offsetHighThreshold = 40;
    this->minAreaBlob = 1000;
    this->maxAreaBlob = 50000;

    this->params.minThreshold = 10;
    this->params.maxThreshold = 200;
    this->params.filterByArea = true;
    this->params.minArea = minAreaBlob;
    this->params.maxArea = maxAreaBlob;

    cv::namedWindow(MASK_TAG, CV_WINDOW_NORMAL);
}

ColorDetector::~ColorDetector()
{
}

cv::Point2f ColorDetector::getCurrentPoint()
{
    return this->currentPoint;
}

void ColorDetector::loop(cv::Mat frame, bool showROI)
{
	tigl::shader->enableColor(false);
	tigl::shader->enableTexture(true);

	if (showROI) {
		drawROI(frame);

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			std::cout << "S KEY PRESSED > Color detection running...\n";
			calculateTresholds();

			// Create window for controls DEBUG
			// createSettingsWindow();

			this->colorDetected = true;
		}
	}

	if (this->colorDetected) {
		applyTransformations(frame);
		imshow(MASK_TAG, mask * 255);

		// Get blobs from mask
		std::vector<cv::KeyPoint> keypoints;
		cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
		detector->detect(mask, keypoints);

		// Draw around object
		//drawKeypoints(frame, keypoints, frame, cv::Scalar(0, 255, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		for (int i = 0; i < keypoints.size(); i++) {
			calculateNewPoint(keypoints, i, frame);
			circle(frame, keypoints[i].pt, 25, cv::Scalar(0, 255, 255), -1, 6, 0);
		}
	}

	bindTexture(frame);
	drawTexPane();
}

void ColorDetector::drawROI(cv::Mat frame)
{
	// Size of ROI for color selecting
	cv::Rect rLeft((frame.size().width / 2) - 40, (frame.size().height / 2) - 20, 40, 40);
	cv::Rect rRight((frame.size().width / 2) + 10, (frame.size().height / 2) - 20, 40, 40);

	// Cropping ROI from main frame
	croppedImageL = frame(rLeft);
	croppedImageR = frame(rRight);

	// Drawing ROI rectangles
	rectangle(frame, rLeft, cv::Scalar(0, 255, 0), 1.5, 8, 0);
	rectangle(frame, rRight, cv::Scalar(0, 255, 0), 1.5, 8, 0);
}

void ColorDetector::drawTexPane()
{
	auto camModel = glm::mat4(1.0f);
	camModel = glm::translate(camModel, glm::vec3(-1.0f, -0.55f, -1.0f));
	tigl::shader->setModelMatrix(camModel);

	tigl::begin(GL_QUADS);
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec2(1, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(2.0f, 1.1f, 0.0f), glm::vec2(1, -1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(0.0f, 1.1f, 0.0f), glm::vec2(0, -1)));
	tigl::end();
}

void ColorDetector::bindTexture(cv::Mat frame)
{
	cv::cvtColor(frame, frame, CV_BGR2RGB);
	glBindTexture(GL_TEXTURE_2D, camTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void ColorDetector::calculateTresholds()
{
	cvtColor(croppedImageL, lHSV, CV_BGR2HSV);
	cvtColor(croppedImageR, rHSV, CV_BGR2HSV);

	// Calculate average, left and right array
	cv::Scalar hsvMeansSample1 = mean(lHSV);
	cv::Scalar hsvMeansSample2 = mean(rHSV);

	// Pick Highest number and apply offset
	hHighThreshold = cv::max(hsvMeansSample1[0], hsvMeansSample2[0]) + (offsetHighThreshold / 2);
	hLowThreshold = cv::min(hsvMeansSample1[0], hsvMeansSample2[0]) - (offsetLowThreshold / 2);

	sHighThreshold = cv::max(hsvMeansSample1[1], hsvMeansSample2[1]) + offsetHighThreshold;
	sLowThreshold = cv::min(hsvMeansSample1[1], hsvMeansSample2[1]) - offsetLowThreshold;

	vHighThreshold = cv::max(hsvMeansSample1[2], hsvMeansSample2[2]) + offsetHighThreshold;
	vLowThreshold = cv::min(hsvMeansSample1[2], hsvMeansSample2[2]) - offsetLowThreshold;
}

void ColorDetector::createSettingsWindow()
{
	cv::namedWindow("Controls", CV_WINDOW_NORMAL);

	// Create all trackbars that the user can control to perfect the output
	cv::createTrackbar("hMin", CONTROLS_TAG, &hLowThreshold, 179);
	cv::createTrackbar("hMax", CONTROLS_TAG, &hHighThreshold, 179);
	cv::createTrackbar("sMin", CONTROLS_TAG, &sLowThreshold, 255);
	cv::createTrackbar("sMax", CONTROLS_TAG, &sHighThreshold, 255);
	cv::createTrackbar("vMin", CONTROLS_TAG, &vLowThreshold, 255);
	cv::createTrackbar("vMax", CONTROLS_TAG, &vHighThreshold, 255);
	cv::createTrackbar("bMin", CONTROLS_TAG, &minAreaBlob, 30000);
	cv::createTrackbar("bMax", CONTROLS_TAG, &maxAreaBlob, 50000);
}

void ColorDetector::applyTransformations(cv::Mat frame)
{
	cvtColor(frame, hsvImage, CV_BGR2HSV);

	inRange(hsvImage,
		cv::Scalar(hLowThreshold, sLowThreshold, vLowThreshold),
		cv::Scalar(hHighThreshold, sHighThreshold, vHighThreshold), mask);

	blur(mask, mask, cv::Size(3, 3));

	morphologyEx(mask, mask, cv::MORPH_OPEN, element);
	dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 3);

	bitwise_not(mask, mask);
}

void ColorDetector::calculateNewPoint(std::vector<cv::KeyPoint> keypoints, int i, cv::Mat frame)
{
	// Make x0 y0 in middle of screen
	cv::Point2f pRaw(keypoints[i].pt.x - (frame.size().width / 2), keypoints[i].pt.y - (frame.size().height / 2));
	if (pRaw.y < 0) {
		pRaw.y -= pRaw.y * 2;
	}
	else {
		pRaw.y *= -1;
	}

	cv::Point2f pFormat(pRaw.x, pRaw.y);
	this->currentPoint = cv::Point2f(pFormat.x, pFormat.y);
}
