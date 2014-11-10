#include "threeDGraphics.h"

ThreeDGraphics::ThreeDGraphics(std::string id) : IComponent(id)
{
	//Invalidate our IDs
	verticesID = 0;
	coloursID = 0;
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

bool ThreeDGraphics::BufferDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr)
{
	//Generate, bind and upload data
	glGenBuffers(1, &bufferAddr);
	glBindBuffer(GL_ARRAY_BUFFER, bufferAddr);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GL_FLOAT), &(data)[0], GL_STATIC_DRAW);

	return true;
}

bool ThreeDGraphics::GenVertexArrays(GLuint bufferAddr, GLuint &arrayAddr)
{
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

//const GLuint ThreeDGraphics::GetVAO()
//{
//	return ;
//}


const std::vector<GLfloat> ThreeDGraphics::GetVertices()
{
	return vertices;
}

const std::vector<GLfloat> ThreeDGraphics::GetColours()
{
	return colours;
}
