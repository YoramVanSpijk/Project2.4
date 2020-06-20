#include "GameObject.h"
#include "Component.h"
#include "DrawComponent.h"

#include <glm\ext\matrix_transform.hpp>
#include "tigl.h"

GameObject::GameObject(Component* component, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
	: position(pos), rotation(rotation), scale(scale)
{
	this->addComponent(component);
}

GameObject::~GameObject()
{
}

void GameObject::addComponent(Component* component)
{
	component->setGameObject(this);
	components.push_back(component);

	if (!drawComponent)
		drawComponent = dynamic_cast<DrawComponent*>(component);
}

void GameObject::removeComponent(Component* component)
{
	components.erase(std::remove(components.begin(), components.end(), component), components.end());
}

std::vector<Component*> GameObject::getComponents()
{
	return std::vector<Component*>();
}

void GameObject::update(float deltaTime)
{
	for (size_t i = 0; i < components.size(); i++)
		components[i]->update(deltaTime);
}

void GameObject::draw()
{
	if (!drawComponent)
		return;

	glm::mat4 modelMatrix = glm::mat4();
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, scale);

	tigl::shader->setModelMatrix(modelMatrix);
	drawComponent->draw();
}