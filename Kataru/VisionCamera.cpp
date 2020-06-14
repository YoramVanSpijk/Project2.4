#include "VisionCamera.h"
#include <atomic>         // std::atomic, std::atomic_flag, ATOMIC_FLAG_INIT
#include <thread>         // std::thread, std::this_thread::yield
#include <iostream>

VisionCamera::VisionCamera(GLFWwindow* window)
{
	this->window = window;
	this->currentPoint = cv::Point2f(0, 0);

    cv::VideoCapture c(0);
    cap = c;
    if (!cap.isOpened())
    {
        std::cout << "Cannot open the video cam" << std::endl;
		this->noCameraError = false;
    }

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
}


VisionCamera::~VisionCamera()
{
}

bool VisionCamera::getCameraError()
{
	return this->noCameraError;
}

cv::Point2f VisionCamera::getCurrentPoint()
{
	return this->currentPoint;
}


void VisionCamera::update(float deltaTime)
{

}

void VisionCamera::draw()
{
	tigl::shader->enableColor(false);
	tigl::shader->enableTexture(true);

	if (cap.read(frame)) {
		flip(frame, frame, 3);

		// Size of ROI for color selecting
		cv::Rect rLeft = cv::Rect((frame.size().width / 2) - 10, (frame.size().height / 2) - 20, 20, 40);
		cv::Rect rRight = cv::Rect((frame.size().width / 2) + 10, (frame.size().height / 2) - 20, 20, 40);

		// Cropping ROI from main frame
		croppedImageL = frame(rLeft);
		croppedImageR = frame(rRight);

		// Drawing ROI rectangles
		rectangle(frame, rLeft, cv::Scalar(0, 255, 0), 1.5, 8, 0);
		rectangle(frame, rRight, cv::Scalar(0, 255, 0), 1.5, 8, 0);

		if (this->colorDetected) {
			cvtColor(frame, hsvImage, CV_BGR2HSV);

			inRange(hsvImage,
				cv::Scalar(hLowThreshold, sLowThreshold, vLowThreshold),
				cv::Scalar(hHighThreshold, sHighThreshold, vHighThreshold), mask);

			blur(mask, mask, cv::Size(3, 3));

			morphologyEx(mask, mask, cv::MORPH_OPEN, element);
			dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 3);

			bitwise_not(mask, mask);

			imshow(MASK_TAG, mask * 255);

			std::vector<cv::KeyPoint> keypoints;
			cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
			detector->detect(mask, keypoints);
			drawKeypoints(frame, keypoints, frame, cv::Scalar(255, 0, 0), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

			for (int i = 0; i < keypoints.size(); i++) {
				cv::Point2f pRaw = cv::Point2f(keypoints[i].pt.x - (frame.size().width / 2), keypoints[i].pt.y - (frame.size().height / 2));

				if (pRaw.y < 0) {
					pRaw.y -= pRaw.y * 2;
				}
				else {
					pRaw.y *= -1;
				}

				cv::Point2f pFormat = cv::Point2f(pRaw.x, pRaw.y);
				
				std::cout << "X: " << pFormat.x << ", Y: " << pFormat.y << "\n";
				this->currentPoint = cv::Point2f(pFormat.x, pFormat.y);

				circle(frame, keypoints[i].pt, 4, cv::Scalar(0, 0, 255), -1, 8, 0);
			}
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			std::cout << "S KEY PRESSED > Color detection running...\n";

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

			// Create window for controls
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

			this->colorDetected = true;
		}

		cvtColor(frame, frame, CV_BGR2RGB);
		glBindTexture(GL_TEXTURE_2D, camTexId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	glm::mat4 camModel = glm::mat4(1.0f);
	camModel = glm::translate(camModel, glm::vec3(-1.0f, -0.5f, -1.0f));
	tigl::shader->setModelMatrix(camModel);

	tigl::begin(GL_QUADS);
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec2(1, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(2.0f, 1.0f, 0.0f), glm::vec2(1, -1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, -1)));
	tigl::end();
}
