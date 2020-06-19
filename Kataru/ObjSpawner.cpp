#include "ObjSpawner.h"
#include "ObjModel.h"
#include <iostream>
#include <chrono>
#include <stdio.h>  
#include <stdlib.h> 
#include <time.h>   

ObjSpawner::ObjSpawner()
{
    this->thread = std::thread(&ObjSpawner::spawn, this);
}

ObjSpawner::~ObjSpawner()
{
    this->thread.join();
}

void ObjSpawner::attachGameObject(GameObject* obj)
{
    gameObjects.push_back(obj);
}

void ObjSpawner::removeGameObject(GameObject* obj)
{
    for (size_t i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i] == obj) {
            gameObjects.erase(gameObjects.begin() + i);
        }
    }
}

void ObjSpawner::spawn(ObjSpawner* spawner)
{
    srand(time(NULL));
    while (true)
    {
        GameObject* object = new GameObject(new ObjModel("models/car/honda_jazz.obj"), glm::vec3(0, 0, -10), glm::vec3(0, 0, 0), glm::vec3(0.01f, 0.01f, 0.01f));
        spawner->attachGameObject(object);

        glm::vec3 velocity(ObjSpawner::fRand(-0.2, 0.2), 1, 0.5f);
        for (int i = 0; object->position.y >= 0; i++)
        {
            object->position = glm::vec3(object->position.x += velocity.x, object->position.y += velocity.y, object->position.z += velocity.z);
            velocity.y -= 0.01 * i;

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        spawner->removeGameObject(spawner->gameObjects[0]);
        delete object;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

double ObjSpawner::fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
    printf("");
}

void ObjSpawner::update(double deltaTime)
{
	for (size_t i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->update(deltaTime);
}

void ObjSpawner::draw()
{
	for (size_t i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->draw();
}

