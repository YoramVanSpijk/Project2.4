#pragma once

#include <vector>
#include <memory>
#include "GameObject.h"

class ObjSpawner
{
public:
	ObjSpawner();

	void attachGameObject(Component* component, glm::vec3 pos, glm::vec3 rotation, float scale);

	void update(double deltaTime);
	void draw();

private:
	std::vector<GameObject*> gameObjects;
};

