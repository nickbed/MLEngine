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
	void GetAllData(std::vector<GLfloat> &verts, std::vector<GLfloat> &UVs, std::vector<GLfloat> &normals, std::vector<GLuint> &indices);

	//Parsed data
	const std::vector<GLfloat> GetVertices();
	const std::vector<GLfloat> GetUVs();
	const std::vector<GLfloat> GetNormals();
	const std::vector<GLuint> GetIndicies();
	

private:
	std::vector<GLfloat> parsedVertices;
	std::vector<GLfloat> parsedUVs;
	std::vector<GLfloat> parsedNormals;
	std::vector<GLuint> parsedIndices;
};
#endif