#pragma once
#include "GameObject.h"

class Component
{
protected:
	GameObject* gameObject;

public:
	Component();
	~Component();

	virtual void update(float deltaTime) {};

	inline void setGameObject(GameObject* gameobject)
	{
		this->gameObject = gameObject;
	}
};