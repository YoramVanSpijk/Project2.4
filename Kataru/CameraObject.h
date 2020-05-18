#pragma once

#include "DrawComponent.h"
#include "FpsCam.h"
#include <GLFW\glfw3.h>

class CameraObject
{
	std::vector<FpsCam*> cameras;
	DrawComponent* drawComponent = nullptr;

public:
	CameraObject(GLFWwindow* window);
	~CameraObject();

	glm::mat4 getMatrix();

	void addCamera(FpsCam* camera);
	void removeCamera(FpsCam* camera);
	std::vector<FpsCam*> getCameras();

	void update(GLFWwindow* window);
	void draw();
};