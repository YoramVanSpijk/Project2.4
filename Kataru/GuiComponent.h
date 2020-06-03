#pragma once

#include <stdio.h>
#include "GuiObject.h"

class GuiComponent
{
protected:
	GuiObject* parentObject;

public:
	GuiComponent();
	~GuiComponent();

	virtual void draw(GLFWwindow* window) {};
	virtual void update(float deltaTime) {};

	inline void setGameObject(GuiObject* parentObject)
	{
		this->parentObject = parentObject;
	}
};