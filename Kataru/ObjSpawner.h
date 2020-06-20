#pragma once

#include <vector>
#include <memory>
#include <thread>
#include "GameObject.h"
#include "Difficulty.h"

class ObjSpawner
{
public:
	ObjSpawner();
	~ObjSpawner();

	void attachGameObject(GameObject* obj);
	void removeGameObject(GameObject* obj);
	void throwObject(int i);

	void setDifficulty(Difficulty difficulty);

	void update(double deltaTime);
	void draw();

private:
	std::vector<GameObject*> gameObjects;
	std::thread thread;
	Difficulty difficulty;
	static void spawn(ObjSpawner* spawner);
	static double fRand(double fMin, double fMax);
};

