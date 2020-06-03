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
	GameObject();
	~GameObject();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void addComponent(Component* component);
	void removeComponent(Component* component);
	std::vector<Component*> getComponents();

	void update(float deltaTime);
	void draw(const glm::mat4& = glm::mat4(1.0f));
};