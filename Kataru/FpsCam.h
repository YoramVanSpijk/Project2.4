#pragma once
#include "DrawComponent.h"
#include "tigl.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "FpsCam.h"

struct GLFWwindow;

class FpsCam : public DrawComponent
{
public:
	FpsCam(GLFWwindow* window);

	glm::mat4 getMatrix();

	void update(GLFWwindow* window);

private:
	GLuint texId = -3;

	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec2 rotation = glm::vec2(0, 0);

	void move(float angle, float fac);
	void moveVertical(float y);
};


