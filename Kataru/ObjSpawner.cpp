#include "ObjSpawner.h"
#include "ObjModel.h"

ObjSpawner::ObjSpawner()
{
    this->attachGameObject(new ObjModel("models/car/honda_jazz.obj"), glm::vec3(3, 0, 0), glm::vec3(5, 5, 0), 0.01f);
    this->attachGameObject(new ObjModel("models/car/honda_jazz.obj"), glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), 0.01f);
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
