#include "StaticMeshNoIndices.h"

StaticMeshNoIndices::StaticMeshNoIndices(std::string id) : IComponent(id)
{
	//Invalidate our IDs
	verticesID = 0;
	coloursID = 0;
	normalsID = 0;
	vaoID = 0;
	currentVAOIndex = 0;
	VAOGenerated = false;
}

StaticMeshNoIndices::~StaticMeshNoIndices()
{
	IComponent::~IComponent();
}

void StaticMeshNoIndices::Init()
{
}

void StaticMeshNoIndices::Update(float dt)
{
}

void StaticMeshNoIndices::Destroy()
{
}

bool StaticMeshNoIndices::UploadVertices(std::vector<GLfloat> verts)
{
	bool result = BufferDataToGPU(verts, verticesID);
	if (result)
	{
		vertices = verts;
	}
	return result;
}

bool StaticMeshNoIndices::UploadColours(std::vector<GLfloat> col)
{
	bool result = BufferDataToGPU(col, coloursID);
	if (result) colours = col;
	return result;
}

bool StaticMeshNoIndices::UploadNormals(std::vector<GLfloat> norm)
{
	bool result = BufferDataToGPU(norm, normalsID);
	if (result) normals = norm;
	return result;
}

bool StaticMeshNoIndices::BufferDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr)
{
	GenVertexArrays(bufferAddr, vaoID, currentVAOIndex);

	//Generate, bind and upload data
	glGenBuffers(1, &bufferAddr);
	glBindBuffer(GL_ARRAY_BUFFER, bufferAddr);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GL_FLOAT), &(data)[0], GL_STATIC_DRAW);
	glVertexAttribPointer(currentVAOIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	++currentVAOIndex;

	GLenum error=glGetError();
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	return true;
}

bool StaticMeshNoIndices::GenVertexArrays(GLuint bufferAddr, GLuint &arrayAddr, int &currentIndex)
{
	if(!VAOGenerated)
	{
		glGenVertexArrays(1, &arrayAddr);
		VAOGenerated = true;
	}
	glBindVertexArray(arrayAddr);

	return true;
}

const GLuint StaticMeshNoIndices::GetVerticesID()
{
	return verticesID;
}

const GLuint StaticMeshNoIndices::GetColoursID()
{
	return coloursID;
}

const GLuint StaticMeshNoIndices::GetNormalsID()
{
	return normalsID;
}

const GLuint StaticMeshNoIndices::GetVAO()
{
	return vaoID;
}


const std::vector<GLfloat> StaticMeshNoIndices::GetVertices()
{
	return vertices;
}

const std::vector<GLfloat> StaticMeshNoIndices::GetColours()
{
	return colours;
}

const std::vector<GLfloat> StaticMeshNoIndices::GetNormals()
{
	return normals;
}
