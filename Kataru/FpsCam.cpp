#include "FpsCam.h"

FpsCam::FpsCam(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	// Open de camera met nummer 1 in lijst (red.: nr 0 was bij mij de camera in de klep van mijn laptop)  
	cv::VideoCapture c(0);
	cap = c;

	// Controle of de camera wordt herkend.
	if (!cap.isOpened())
		std::cout << "Cannot open the video cam" << std::endl;

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
}


glm::mat4 FpsCam::getMatrix()
{
	glm::mat4 ret(1.0f);
	ret = glm::rotate(ret, rotation.x, glm::vec3(1, 0, 0));
	ret = glm::rotate(ret, rotation.y, glm::vec3(0, 1, 0));
	ret = glm::translate(ret, position);
	return ret;
}

void FpsCam::move(float angle, float fac)
{
	position.x += (float)cos(rotation.y + glm::radians(angle)) * fac;
	position.z += (float)sin(rotation.y + glm::radians(angle)) * fac;
}

void FpsCam::draw()
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(getMatrix());
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(false);
	tigl::shader->enableTexture(true);

	cv::Mat frame;
	if (cap.read(frame)) {
		flip(frame, frame, 3);

		cvtColor(frame, frame, CV_BGR2RGB);

		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	tigl::begin(GL_QUADS);
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec2(1, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(2.0f, 1.0f, 0.0f), glm::vec2(1, -1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0, -1)));
	tigl::end();
}

void FpsCam::update(GLFWwindow* window)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	static double lastX = x;
	static double lastY = y;

	rotation.x -= (float)(lastY - y) / 100.0f;
	rotation.y -= (float)(lastX - x) / 100.0f;

	lastX = x;
	lastY = y;


	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		move(0, 0.2f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		move(180, 0.2f);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		move(90, 0.2f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		move(-90, 0.2f);
}
