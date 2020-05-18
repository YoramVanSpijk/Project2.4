#include "CameraObject.h"

CameraObject::CameraObject(GLFWwindow*)
{
}

CameraObject::~CameraObject()
{
}

glm::mat4 CameraObject::getMatrix()
{
	return glm::mat4();
}

void CameraObject::addCamera(FpsCam* camera)
{
	cameras.push_back(camera);

	if (!drawComponent)
		drawComponent = dynamic_cast<DrawComponent*>(camera);
}

void CameraObject::removeCamera(FpsCam* camera)
{
	cameras.erase(std::remove(cameras.begin(), cameras.end(), camera), cameras.end());
}

std::vector<FpsCam*> CameraObject::getCameras()
{
	return std::vector<FpsCam*>();
}

void CameraObject::update(GLFWwindow* window)
{
	for (size_t i = 0; i < cameras.size(); i++)
		cameras[i]->update(window);
}

void CameraObject::draw()
{
	for (size_t i = 0; i < cameras.size(); i++)
		cameras[i]->draw();
}