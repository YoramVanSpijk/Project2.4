#pragma once

#include <opencv2/opencv.hpp>

void onDraw(void* param);

class ARWindow
{
public:
	ARWindow(std::string windowName, cv::Size frameSize);
	~ARWindow();

	void updateBackground(const cv::Mat& frame);
	void updateWindow();
	void draw();
	void drawBackground();
	void draw3DLayer();

private:
	std::string windowName;
	cv::Size frameSize;
	cv::Mat background;
};

