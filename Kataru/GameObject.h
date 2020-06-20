#pragma once

#include <glm/glm.hpp>
#include <vector>

class DrawComponent;
class Component;

class GameObject
{
	std::vector<Component*> components;
	DrawComponent* drawComponent = nullptr;

public:
	GameObject(Component* component, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale);
	~GameObject();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void addComponent(Component* component);
	void removeComponent(Component* component);
	std::vector<Component*> getComponents();

	void update(float deltaTime);
	void draw();
};