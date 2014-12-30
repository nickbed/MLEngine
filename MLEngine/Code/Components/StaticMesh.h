#ifndef STATICMESHCOMPONENT_H
#define STATICMESHCOMPONENT_H
#include "..\Interfaces\IComponent.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"


#include <string>
#include <vector>

class StaticMesh : public IComponent
{
public:
	StaticMesh(std::string id);
	virtual ~StaticMesh();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();

	bool UploadVertices(std::vector<GLfloat> vertices);
	bool UploadNormals(std::vector<GLfloat> normals);
	bool UploadUVs(std::vector<GLfloat> UVs);
	bool UploadIndices(std::vector<GLuint> indices);

	bool BufferDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr);
	bool BufferDataToGPU(std::vector<GLuint> data, GLuint &bufferAddr);
	bool GenVertexArrays(GLuint bufferAddr, GLuint &arrayAddr, int &currentIndex);

	const GLuint GetVerticesID();
	const GLuint GetNormalsID();
	const GLuint GetUVsID();
	const GLuint GetIndicesID();
	const GLuint GetVAO();

	const std::vector<GLfloat> GetVertices();
	const std::vector<GLfloat> GetNormals();
	const std::vector<GLfloat> GetUVs();
	const std::vector<GLuint> GetIndices();
	//TODO - Normals/texture support

private:
	GLuint verticesID;
	GLuint normalsID;
	GLuint ID;
	GLuint vaoID;
	GLuint uvID;
	GLuint indicesID;

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> UVs;
	std::vector<GLuint> indices;
	bool VAOGenerated;
	int currentVAOIndex;
};

#endif