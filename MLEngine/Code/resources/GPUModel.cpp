#include "GPUModel.h"



GPUModel::GPUModel()
{
	//Invalidate our IDs
	verticesID = 0;
	normalsID = 0;
	uvID = 0;
	indicesID = 0;
	vaoID = 0;
	VAOGenerated = false;
}

GPUModel::~GPUModel()
{
}

bool GPUModel::UploadData()
{
	glBindVertexArray(0);
	//Vertices
	mauveassert::Assert::AssertTrue("Vertices data null", (verticesSize > 0), mauveassert::ENUM_severity::SEV_FATAL);
	bool result = BufferDataToGPU(vertices, verticesID, 0, verticesSize);
	if (!result) return false;

	//Normals
	mauveassert::Assert::AssertTrue("Normals data null", (normalsSize > 0), mauveassert::ENUM_severity::SEV_FATAL);
	result = BufferDataToGPU(normals, normalsID, 1, normalsSize);
	if (!result) return false;

	//UVs
	mauveassert::Assert::AssertTrue("UV data null", (UVsSize > 0), mauveassert::ENUM_severity::SEV_FATAL);
	result = BufferUVDataToGPU(UVs, uvID, UVsSize);
	if (!result) return false;

	//Indices
	mauveassert::Assert::AssertTrue("Indices data null", (indicesSize > 0), mauveassert::ENUM_severity::SEV_FATAL);
	result = BufferDataToGPU(indices, indicesID, indicesSize);
	if (result)
	{
		bufferloc = vaoID;
		loaded = true;
		return true;
	}
	return false;
}

bool GPUModel::DeleteData()
{
	glDeleteBuffers(1, &verticesID);
	glDeleteBuffers(1, &normalsID);
	glDeleteBuffers(1, &uvID);
	glDeleteBuffers(1, &indicesID);
	glDeleteVertexArrays(1, &vaoID);
	return true;
}

bool GPUModel::SetAllData(GLfloat *verts, unsigned int vertSize, GLfloat *norms, unsigned int  normSize, GLfloat *UV, unsigned int  UVSize, GLuint *inds, unsigned int  indSize)
{
	memcpy(vertices, verts, vertSize * sizeof(GLfloat));
	//*vertices = *verts;
	verticesSize = vertSize;

	memcpy(normals, norms, normSize * sizeof(GLfloat));
	//*normals = *norms;
	normalsSize = normSize;

	memcpy(UVs, UV, UVSize * sizeof(GLfloat));
	//*UVs = *UV;
	UVsSize = UVSize;

	memcpy(indices, inds, indSize * sizeof(GLuint));
	//*indices = *inds;
	indicesSize = indSize;

	return true;
}

bool GPUModel::SetVertices(GLfloat  *verts, unsigned int size)
{
	verticesSize = size;
	memcpy(vertices, verts, size * sizeof(GLfloat));
	//*vertices = *verts;
	return true;
}

bool GPUModel::SetNormals(GLfloat  *norms, unsigned int size)
{
	normalsSize = size;
	memcpy(normals, norms, size * sizeof(GLfloat));
	//*normals = *norms;
	return true;
}

bool GPUModel::SetUVs(GLfloat  *UV, unsigned int size)
{
	UVsSize = size;
	memcpy(UVs, UV, size * sizeof(GLfloat));
	//*UVs = *UV;
	return true;
}

bool GPUModel::SetIndices(GLuint * inds, unsigned int size)
{
	indicesSize = size;
	memcpy(indices, inds, size * sizeof(GLuint));
	//*indices = *inds;
	return true;
}

GLfloat  *GPUModel::GetVertices()
{
	return vertices;
}

GLfloat  *GPUModel::GetNormals()
{
	return normals;
}

GLfloat  *GPUModel::GetUVs()
{
	return UVs;
}

GLuint *GPUModel::GetIndices()
{
	return indices;
}

const GLuint GPUModel::GetVerticesID()
{
	return verticesID;
}

const GLuint GPUModel::GetNormalsID()
{
	return normalsID;
}

const GLuint GPUModel::GetUVsID()
{
	return uvID;
}

const GLuint GPUModel::GetIndicesID()
{
	return indicesID;
}

const GLuint GPUModel::GetVAO()
{
	return vaoID;
}

const unsigned int GPUModel::GetVerticesCount()
{
	return verticesSize;
}

const unsigned int GPUModel::GetIndicesCount()
{
	return indicesSize;
}

bool GPUModel::BufferDataToGPU(GLfloat  * data, GLuint &bufferAddr, GLuint currentLoc, unsigned int size)
{
	glEnableVertexAttribArray(currentLoc);
	GenVertexArrays(bufferAddr, vaoID);

	//Generate, bind and upload data
	glEnableVertexAttribArray(currentLoc);
	glGenBuffers(1, &bufferAddr);

	glBindBuffer(GL_ARRAY_BUFFER, bufferAddr);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(GL_FLOAT), &(data)[0], GL_STATIC_DRAW);

	glVertexAttribPointer(currentLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	return true;
}

bool GPUModel::BufferDataToGPU(GLuint * data, GLuint &bufferAddr, unsigned int size)
{
	GenVertexArrays(bufferAddr, vaoID);
	//Generate, bind and upload data
	glGenBuffers(1, &bufferAddr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferAddr);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), &(data)[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	return true;
}

bool GPUModel::BufferUVDataToGPU(GLfloat  * data, GLuint &bufferAddr, unsigned int size)
{
	GenVertexArrays(bufferAddr, vaoID);
	//Generate, bind and upload data
	glEnableVertexAttribArray(2);
	glGenBuffers(1, &bufferAddr);

	glBindBuffer(GL_ARRAY_BUFFER, bufferAddr);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(GL_FLOAT), &(data)[0], GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	return true;
}

bool GPUModel::GenVertexArrays(GLuint bufferAddr, GLuint &arrayAddr)
{
	if (!VAOGenerated)
	{
		glGenVertexArrays(1, &arrayAddr);
		VAOGenerated = true;
	}
	glBindVertexArray(arrayAddr);

	return true;
}



