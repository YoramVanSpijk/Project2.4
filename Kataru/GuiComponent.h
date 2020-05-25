#pragma once
#include "DrawComponent.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"

class GuiComponent : public DrawComponent
{
	GuiComponent();
public:
	void draw();
};

