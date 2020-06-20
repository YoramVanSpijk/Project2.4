#include "ObjSpawner.h"
#include "ObjModel.h"
#include <iostream>
#include <chrono>
#include <stdio.h>  
#include <stdlib.h> 
#include <time.h>   
#include <glm\ext\matrix_transform.hpp>

ObjSpawner::ObjSpawner()
    : difficulty(Difficulty::VETERAN)
{
    this->thread = std::thread(&ObjSpawner::spawn, this);
    srand(time(NULL));
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

void ObjSpawner::throwObject(int i)
{
    GameObject* object = nullptr;

    if (i == 0)
        object = new GameObject(new ObjModel("models/car/honda_jazz.obj"), glm::vec3(0, 0, -10), glm::vec3(0, 0, 0), glm::vec3(0.01f, 0.01f, 0.01f));
    else if (i == 1)
        object = new GameObject(new ObjModel("models/steve/Steve.obj"), glm::vec3(0, 0, -10), glm::vec3(0, glm::half_pi<float>(), 0), glm::vec3(0.1f, 0.1f, 0.1f));
    else if (i == 2)
        object = new GameObject(new ObjModel("models/cube/cube-textures.obj"), glm::vec3(0, 0, -10), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
   
    if (!object)
        return;

    this->attachGameObject(object);

    glm::vec3 velocity(ObjSpawner::fRand(-0.2, 0.2), 1, 0.5f);
    for (int i = 0; object->position.y >= 0; i++)
    {
        object->position = glm::vec3(object->position.x += velocity.x, object->position.y += velocity.y, object->position.z += velocity.z);
        velocity.y -= 0.01 * i;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    this->removeGameObject(object);
    delete object;
}

void ObjSpawner::setDifficulty(Difficulty difficulty)
{
    this->difficulty = difficulty;
}

void ObjSpawner::spawn(ObjSpawner* spawner)
{
    while (true)
    {
        for (int i = 0; i < 3; i++)
        {
            spawner->throwObject(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(spawner->difficulty)));
        }
    }
}


double ObjSpawner::fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

//void ObjSpawner::update(double deltaTime)
//{
//	for (size_t i = 0; i < gameObjects.size(); i++)
//		gameObjects[i]->update(deltaTime);
//}
//
//void ObjSpawner::draw()
//{
//	for (size_t i = 0; i < gameObjects.size(); i++)
//		gameObjects[i]->draw();
//}

