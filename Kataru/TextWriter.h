#pragma once
#include <string>
#include <glm\fwd.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "tigl.h"



class TextWriter
{
public:
	static TextWriter* getInstance();
	~TextWriter();
	void writeText(glm::vec3, std::string);

private:
	TextWriter();
	static TextWriter* instance;
	void initWriter();
	//GLuint texId;
	//stbtt_bakedchar cdata[96];
};

