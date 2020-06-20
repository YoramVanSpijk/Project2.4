#pragma once

#include <vector>
#include <memory>
#include <thread>
#include "GameObject.h"
#include "Difficulty.h"
#include <glm\detail\type_vec3.hpp>

class ObjSpawner
{
public:
	ObjSpawner();
	~ObjSpawner();

	void setDifficulty(Difficulty difficulty);
	void setOn(bool on);

	void update(double deltaTime);
	void draw();
	std::vector<GameObject*> getObjects();

private:
	std::vector<GameObject*> gameObjects;
	std::thread thread;

	Difficulty difficulty;
	bool on;

	static void spawn(ObjSpawner* spawner);
	static double fRand(double fMin, double fMax);
	void attachGameObject(GameObject* obj);
	void removeGameObject(GameObject* obj);
	void throwObject(int i);
};

