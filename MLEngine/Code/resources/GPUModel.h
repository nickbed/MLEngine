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

	bool SetAllData(GLfloat *vertices, unsigned int vertSize, GLfloat *normals, unsigned int  normalsSize, GLfloat *UVs, unsigned int  UVsSize, unsigned int indexSize);
	//Custom model stuff
	bool SetVertices(GLfloat *vertices, unsigned int  vertSize);
	bool SetNormals(GLfloat *normals, unsigned int  normalsSize);
	bool SetUVs(GLfloat *UVs, unsigned int  UVsSize);
	bool SetIndices(GLuint *indices, unsigned int  indexSize);

	GLfloat *GetVertices();
	GLfloat *GetNormals();
	GLfloat *GetUVs();
	GLuint *GetIndices();

	const GLuint GetVerticesID();
	const GLuint GetNormalsID();
	const GLuint GetUVsID();
	const GLuint GetIndicesID();
	const GLuint GetVAO();

	const unsigned int GetVerticesCount();
	const unsigned int GetIndicesCount();

private:

	bool BufferDataToGPU(GLfloat *data, GLuint &bufferAddr, GLuint currentLoc, unsigned int size);
	bool BufferIndexDataToGPU(GLuint &bufferAddr, unsigned int size);
	bool BufferUVDataToGPU(GLfloat *data, GLuint &bufferAddr, unsigned int size);
	bool GenVertexArrays(GLuint bufferAddr, GLuint &arrayAddr);

	GLuint verticesID;
	GLuint normalsID;
	GLuint ID;
	GLuint vaoID;
	GLuint uvID;
	GLuint indicesID;

	GLfloat vertices[100000];
	GLfloat normals[70000];
	GLfloat UVs[70000];
	bool VAOGenerated;
	unsigned int indicesSize;
	unsigned int verticesSize;
	unsigned int normalsSize;
	unsigned int UVsSize;
};

#endif