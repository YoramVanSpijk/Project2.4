#pragma once

#include <vector>
#include <memory>
#include <thread>
#include "GameObject.h"

class ObjSpawner
{
public:
	ObjSpawner();
	~ObjSpawner();

	void attachGameObject(Component* component, glm::vec3 pos, glm::vec3 rotation, float scale);
	void removeGameObject(GameObject* gameObject);

	void update(double deltaTime);
	void draw();

private:
	std::vector<GameObject*> gameObjects;
	std::thread thread;
	static void spawn(ObjSpawner* spawner);
};

