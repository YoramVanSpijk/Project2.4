#include "VisionCamera.h"
#include <iostream>

VisionCamera::VisionCamera()
{
    cv::VideoCapture c(0);
    cap = c;
    if (!cap.isOpened())
    {
        std::cout << "Cannot open the video cam" << std::endl;
		this->noCameraError = false;
    }
}

VisionCamera::~VisionCamera()
{
}

bool VisionCamera::getCameraError()
{
	return this->noCameraError;
}

cv::Mat VisionCamera::getFrame()
{
	return this->frame;
}

void VisionCamera::update(float deltaTime)
{
	cap.read(this->frame);
	flip(frame, frame, 3);
}

void VisionCamera::draw()
{

}