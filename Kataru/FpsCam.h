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
#include "FpsCam.h"

struct GLFWwindow;

class FpsCam : public DrawComponent
{
public:
	FpsCam(GLFWwindow*);

	glm::mat4 getMatrix();

	void draw();
	void update(GLFWwindow* window);

private:
	cv::VideoCapture cap;
	GLuint texId = -3;

	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec2 rotation = glm::vec2(0, 0);

	void move(float angle, float fac);
};


