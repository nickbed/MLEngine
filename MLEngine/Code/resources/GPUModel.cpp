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
	mauveassert::Assert::AssertTrue("Vertices data null", (vertices.size() > 0), mauveassert::ENUM_severity::SEV_FATAL);
	bool result = BufferDataToGPU(vertices, verticesID, 0);
	if (result)
	{
		verticesCount = vertices.size();
	}
	else return false;

	//Normals
	mauveassert::Assert::AssertTrue("Normals data null", (normals.size() > 0), mauveassert::ENUM_severity::SEV_FATAL);
	result = BufferDataToGPU(normals, normalsID, 1);
	if (!result) return false;

	//UVs
	mauveassert::Assert::AssertTrue("UV data null", (UVs.size() > 0), mauveassert::ENUM_severity::SEV_FATAL);
	result = BufferUVDataToGPU(UVs, uvID);
	if (!result) return false;

	//Indices
	mauveassert::Assert::AssertTrue("Indices data null", (indices.size() > 0), mauveassert::ENUM_severity::SEV_FATAL);
	result = BufferDataToGPU(indices, indicesID);
	if (result)
	{
		indicesCount = indices.size();
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

bool GPUModel::SetAllData(std::vector<GLfloat> verts, std::vector<GLfloat> norms, std::vector<GLfloat> UV, std::vector<GLuint> inds)
{
	vertices = verts;
	normals = norms;
	UVs = UV;
	indices = inds;
	return true;
}

bool GPUModel::SetVertices(std::vector<GLfloat> verts)
{
	vertices = verts;
	return true;
}

bool GPUModel::SetNormals(std::vector<GLfloat> norms)
{
	normals = norms;
	return true;
}

bool GPUModel::SetUVs(std::vector<GLfloat> UV)
{
	UVs = UV;
	return true;
}

bool GPUModel::SetIndices(std::vector<GLuint> inds)
{
	indices = inds;
	return true;
}

const std::vector<GLfloat> GPUModel::GetVertices()
{
	return vertices;
}

const std::vector<GLfloat> GPUModel::GetNormals()
{
	return normals;
}

const std::vector<GLfloat> GPUModel::GetUVs()
{
	return UVs;
}

const std::vector<GLuint> GPUModel::GetIndices()
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
	return verticesCount;
}

const unsigned int GPUModel::GetIndicesCount()
{
	return indicesCount;
}

bool GPUModel::BufferDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr, GLuint currentLoc)
{
	glEnableVertexAttribArray(currentLoc);
	GenVertexArrays(bufferAddr, vaoID);

	//Generate, bind and upload data
	glEnableVertexAttribArray(currentLoc);
	glGenBuffers(1, &bufferAddr);

	glBindBuffer(GL_ARRAY_BUFFER, bufferAddr);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GL_FLOAT), &(data)[0], GL_STATIC_DRAW);

	glVertexAttribPointer(currentLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	return true;
}

bool GPUModel::BufferDataToGPU(std::vector<GLuint> data, GLuint &bufferAddr)
{
	GenVertexArrays(bufferAddr, vaoID);
	//Generate, bind and upload data
	glGenBuffers(1, &bufferAddr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferAddr);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), &(data)[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	return true;
}

bool GPUModel::BufferUVDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr)
{
	GenVertexArrays(bufferAddr, vaoID);
	//Generate, bind and upload data
	glEnableVertexAttribArray(2);
	glGenBuffers(1, &bufferAddr);

	glBindBuffer(GL_ARRAY_BUFFER, bufferAddr);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GL_FLOAT), &(data)[0], GL_STATIC_DRAW);

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



