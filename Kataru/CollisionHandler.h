#pragma once
#include "VisionCamera.h"
#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "tigl.h"

class CollisionHandler
{
public:
	CollisionHandler();
	~CollisionHandler();

	void testCollider(GLFWwindow* window, cv::Point2f handPos);
	void check(cv::Point2f handPos, glm::vec3 objPos);
private:
	struct Circle
	{
		float r;
		float x;
		float y;
	};

	float xTestCollider = 0.0f;
	float yTestCollider = 0.0f;

	void drawCircle(float cx, float cy, float r);
	bool isColliding(Circle hand, Circle obj);
};