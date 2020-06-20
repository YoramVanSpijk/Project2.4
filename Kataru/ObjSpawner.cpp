#include "ObjSpawner.h"
#include "ObjModel.h"
#include <iostream>
#include <chrono>
#include <stdio.h>  
#include <stdlib.h> 
#include <time.h>   
#include <glm\ext\matrix_transform.hpp>
#include "tigl.h"

ObjSpawner::ObjSpawner()
    : difficulty(Difficulty::ROOKIE), on(true)
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
    this->gameObjects.push_back(obj);
}

void ObjSpawner::removeGameObject(GameObject* obj)
{
    for (size_t i = 0; i < gameObjects.size(); i++)
    {
        if (this->gameObjects.at(i) == obj) {
            this->gameObjects.erase(this->gameObjects.begin() + i);
        }
    }
}

void ObjSpawner::throwObject(int i)
{
    GameObject* object = nullptr;

    float floor = -0.1f;
    float depth = -0.1f;

    if (i == 0)
        object = new GameObject(new ObjModel("models/car/honda_jazz.obj"), glm::vec3(0, floor, depth), glm::vec3(0, 0, 0), glm::vec3(0.0001f, 0.0001f, 0.0001f));
    else if (i == 1)
        object = new GameObject(new ObjModel("models/steve/Steve.obj"), glm::vec3(0, floor, depth), glm::vec3(0, glm::half_pi<float>(), 0), glm::vec3(0.001f, 0.001f, 0.001f));
    else if (i == 2)
        //object = new GameObject(new ObjModel("models/cube/cube-textures.obj"), glm::vec3(0, floor, depth), glm::vec3(0, 0, 0), glm::vec3(0.01f, 0.01f, 0.01f));
   
    if (!object)
        return;

    this->attachGameObject(object);

    glm::vec3 velocity(ObjSpawner::fRand(-0.001, 0.001), 0.01f, 0); //ObjSpawner::fRand(-0.1, 0.1)
    for (int i = 0; object->position.y >= floor; i++)
    {
        object->position = glm::vec3(object->position.x += velocity.x, object->position.y += velocity.y, object->position.z += velocity.z);
        velocity.y -= 0.0001 * i;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    this->removeGameObject(object);
    delete object;
}

void ObjSpawner::setDifficulty(Difficulty difficulty)
{
    this->difficulty = difficulty;
}

void ObjSpawner::setOn(bool on)
{
    this->on = on;
}

void ObjSpawner::spawn(ObjSpawner* spawner)
{
    while (true)
    {
        if (spawner->on)
        {
            for (int i = 0; i < 3; i++)
            {
                spawner->throwObject(i);
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(spawner->difficulty)));
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

double ObjSpawner::fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void ObjSpawner::update(double deltaTime)
{
    for (auto object : this->gameObjects)
    {
        object->update(deltaTime);
    }
}

std::vector<GameObject*> ObjSpawner::getObjects()
{
    return this->gameObjects;
}

void ObjSpawner::draw()
{
    for (auto object : this->gameObjects)
    {
        object->draw();
    }
}

