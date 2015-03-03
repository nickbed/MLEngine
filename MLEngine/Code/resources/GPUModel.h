//This file is responsible for uploading model(Vertices, indices, normals, VBOs, VAOs) 
//data to the GPU and storing it's location and other attributes
#ifndef GPUModel_H
#define GPUModel_H
#include "..\Assert\Assert.h"
#include "GL\glew.h"
#include "..\Interfaces\IGPUResource.h"
#include <vector>

class GPUModel : public IGPUResource 
{
public:

	//Implementing interface
	GPUModel();
	virtual ~GPUModel();

	virtual bool UploadData();
	virtual bool DeleteData();

	bool SetAllData(std::vector<GLfloat> vertices, std::vector<GLfloat> normals, std::vector<GLfloat> UVs, std::vector<GLuint> indices);
	//Custom model stuff
	bool SetVertices(std::vector<GLfloat> vertices);
	bool SetNormals(std::vector<GLfloat> normals);
	bool SetUVs(std::vector<GLfloat> UVs);
	bool SetIndices(std::vector<GLuint> indices);

	const std::vector<GLfloat> GetVertices();
	const std::vector<GLfloat> GetNormals();
	const std::vector<GLfloat> GetUVs();
	const std::vector<GLuint> GetIndices();

	const GLuint GetVerticesID();
	const GLuint GetNormalsID();
	const GLuint GetUVsID();
	const GLuint GetIndicesID();
	const GLuint GetVAO();

	const unsigned int GetVerticesCount();
	const unsigned int GetIndicesCount();

private:

	bool BufferDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr, GLuint currentLoc);
	bool BufferDataToGPU(std::vector<GLuint> data, GLuint &bufferAddr);
	bool BufferUVDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr);
	bool GenVertexArrays(GLuint bufferAddr, GLuint &arrayAddr);

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
	unsigned int indicesCount;
	unsigned int verticesCount;
};

#endif