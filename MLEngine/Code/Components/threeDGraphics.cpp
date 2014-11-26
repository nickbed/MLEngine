#include "threeDGraphics.h"

ThreeDGraphics::ThreeDGraphics(std::string id) : IComponent(id)
{
	//Invalidate our IDs
	verticesID = 0;
	coloursID = 0;
	normalsID = 0;
	vaoID = 0;
	currentVAOIndex = 0;
	VAOGenerated = false;
}

ThreeDGraphics::~ThreeDGraphics()
{
	IComponent::~IComponent();
}

void ThreeDGraphics::Init()
{
}

void ThreeDGraphics::Update(float dt)
{
}

void ThreeDGraphics::Destroy()
{
}

bool ThreeDGraphics::UploadVertices(std::vector<GLfloat> verts)
{
	bool result = BufferDataToGPU(verts, verticesID);
	if (result)
	{
		vertices = verts;
	}
	return result;
}

bool ThreeDGraphics::UploadColours(std::vector<GLfloat> col)
{
	bool result = BufferDataToGPU(col, coloursID);
	if (result) colours = col;
	return result;
}

bool ThreeDGraphics::UploadNormals(std::vector<GLfloat> norm)
{
	bool result = BufferDataToGPU(norm, normalsID);
	if (result) normals = norm;
	return result;
}

bool ThreeDGraphics::BufferDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr)
{
	//Generate, bind and upload data
	glGenBuffers(1, &bufferAddr);
	glBindBuffer(GL_ARRAY_BUFFER, bufferAddr);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GL_FLOAT), &(data)[0], GL_STATIC_DRAW);

	GenVertexArrays(bufferAddr, vaoID, currentVAOIndex);

	GLenum error=glGetError();

	return true;
}

bool ThreeDGraphics::GenVertexArrays(GLuint bufferAddr, GLuint &arrayAddr, int &currentIndex)
{
	if(!VAOGenerated)
	{
		glGenVertexArrays(1, &arrayAddr);
		VAOGenerated = true;
	}
	glBindVertexArray(arrayAddr);
	glBindBuffer(GL_ARRAY_BUFFER, bufferAddr);
	glVertexAttribPointer (currentIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	++currentIndex;
	return true;
}

const GLuint ThreeDGraphics::GetVerticesID()
{
	return verticesID;
}

const GLuint ThreeDGraphics::GetColoursID()
{
	return coloursID;
}

const GLuint ThreeDGraphics::GetNormalsID()
{
	return normalsID;
}

const GLuint ThreeDGraphics::GetVAO()
{
	return vaoID;
}


const std::vector<GLfloat> ThreeDGraphics::GetVertices()
{
	return vertices;
}

const std::vector<GLfloat> ThreeDGraphics::GetColours()
{
	return colours;
}

const std::vector<GLfloat> ThreeDGraphics::GetNormals()
{
	return normals;
}
