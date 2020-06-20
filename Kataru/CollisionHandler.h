#pragma once
#include "VisionCamera.h"

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