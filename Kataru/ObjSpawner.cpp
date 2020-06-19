#include "ObjSpawner.h"
#include "ObjModel.h"
#include <iostream>
#include <chrono>

ObjSpawner::ObjSpawner()
{
    this->thread = std::thread(&ObjSpawner::spawn, this);
}

ObjSpawner::~ObjSpawner()
{
    this->thread.join();
}

void ObjSpawner::attachGameObject(Component* component, glm::vec3 pos, glm::vec3 rotation, float scale)
{
    GameObject* obj = new GameObject();
    obj->position = pos;
    obj->rotation = rotation;
    obj->scale = glm::vec3(scale, scale, scale);

    obj->addComponent(component);

    gameObjects.push_back(obj);
}

void ObjSpawner::removeGameObject(GameObject* gameObject)
{
    for (size_t i = 0; i < gameObjects.size(); i++)
    {
        if (gameObject == gameObjects[i]) {
            gameObjects.erase(gameObjects.begin() + i);
        }
    }
    delete gameObject;
}

void ObjSpawner::spawn(ObjSpawner* spawner)
{
    int i = 0;
    while (true)
    {
        spawner->attachGameObject(new ObjModel("models/car/honda_jazz.obj"), glm::vec3(i*3, 0, 0), glm::vec3(0, 0, 0), 0.01f);

        if (i % 3 == 0)
        {
            spawner->removeGameObject(spawner->gameObjects[0]);
        }


        i++;

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
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

