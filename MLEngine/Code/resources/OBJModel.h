#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "../Assert/Assert.h"
#include "..\Interfaces\IResource.h"
#include "GL\glew.h"
#include "GLM\glm.hpp"
#include "GLFW\glfw3.h"
#include "..\File\FileManager.h"
#include <string>
#include <vector>

class OBJModel : public IResource
{
public:
	OBJModel();
	OBJModel(std::string path);

	~OBJModel();

	//Load it
	virtual bool LoadFromFile(std::string filename);

	//Return all data by reference (speed)
	//void GetAllData(std::vector<GLfloat> &verts, std::vector<GLfloat> &UVs, std::vector<GLfloat> &normals, std::vector<GLuint> &indices);

	//Parsed data
	GLfloat* GetVertices();
	GLfloat* GetUVs();
	GLfloat* GetNormals();
	GLuint* GetIndicies();

	const unsigned int GetVertexCount();
	const unsigned int GetIndexCount();
	const unsigned int GetNormalCount();
	const unsigned int GetUVCount();
	

private:
	GLfloat parsedVertices[100000];
	GLfloat parsedUVs[70000];
	GLfloat parsedNormals[70000];

	unsigned int parsedUVCount, parsedNormalCount, parsedVertCount, parsedIndexCount;

};
#endif