#pragma once

#include <GL/glew.h>
#include "DrawComponent.h"
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <list>

class Texture;



class ObjModel : public DrawComponent
{
private:
	class Vertex
	{
	public:
		int position;
		int normal;
		int texcoord; 
	};

	class Face
	{
	public:
		std::list<Vertex> vertices;
	};
	class MaterialInfo
	{
	public:
		MaterialInfo();
		std::string name;
		Texture* texture;
	};

	class ObjGroup
	{
	public:
		std::string name;
		int materialIndex;
		std::list<Face> faces;
	};


	std::vector<glm::vec3>	vertices;
	std::vector<glm::vec3>	normals;
	std::vector<glm::vec2>	texcoords;
	std::vector<ObjGroup*> groups;
	std::vector<MaterialInfo*> materials;
	glm::vec4 color;

	void loadMaterialFile(const std::string &fileName, const std::string &dirName);
public:
	ObjModel(const std::string &filename, glm::vec4 color);
	~ObjModel(void);

	void draw();
};

