#include "StaticMesh.h"

StaticMesh::StaticMesh(std::string id) : IComponent(id)
{
	//Invalidate our IDs
	verticesID = 0;
	normalsID = 0;
	uvID = 0;
	indicesID = 0;
	vaoID = 0;
	textureID = 0;
	currentVAOIndex = 0;
	VAOGenerated = false;
}

StaticMesh::~StaticMesh()
{
	IComponent::~IComponent();
	glDeleteBuffers(1, &verticesID);
	glDeleteBuffers(1, &normalsID);
	glDeleteBuffers(1, &uvID);
	glDeleteBuffers(1, &indicesID);
	glDeleteTextures(1, &textureID);

}

void StaticMesh::Init()
{
}

void StaticMesh::Update(float dt)
{
}

void StaticMesh::Destroy()
{
}

bool StaticMesh::UploadVertices(std::vector<GLfloat> verts)
{
	bool result = BufferDataToGPU(verts, verticesID, 0);
	if (result)
	{
		verticesCount = verts.size();
		vertices = verts;
	}

	return result;
}


bool StaticMesh::UploadNormals(std::vector<GLfloat> norm)
{
	bool result = BufferDataToGPU(norm, normalsID, 1);
	if (result) normals = norm;
	return result;
}

bool StaticMesh::UploadUVs(std::vector<GLfloat> uvs)
{
	bool result = BufferUVDataToGPU(uvs, uvID);
	if (result)
	{
		UVs = uvs;
	}
	return result;
}

bool StaticMesh::UploadIndices(std::vector<GLuint> ind)
{
	bool result = BufferDataToGPU(ind, verticesID);
	if (result)
	{
		indicesCount = ind.size();
		indices = ind;
	}
	return result;
}

bool StaticMesh::UploadTexture(Bitmap* texture)
{
	NULLPTRCHECK(texture, "Null pointer passed for texture bitmap");
	bool result = BufferTextureDataToGPU(texture, textureID);
	return result;
}

bool StaticMesh::BufferDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr, GLuint currentLoc)
{

	glEnableVertexAttribArray(currentLoc);
	GenVertexArrays(bufferAddr, vaoID, currentVAOIndex);

	//Generate, bind and upload data
	glEnableVertexAttribArray(currentLoc);
	glGenBuffers(1, &bufferAddr);
	glBindBuffer(GL_ARRAY_BUFFER, bufferAddr);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GL_FLOAT), &(data)[0], GL_STATIC_DRAW);
	glVertexAttribPointer(currentLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL);



	++currentVAOIndex;

	GLenum error = glGetError();
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	return true;
}

bool StaticMesh::BufferDataToGPU(std::vector<GLuint> data, GLuint &bufferAddr)
{

	GenVertexArrays(bufferAddr, vaoID, currentVAOIndex);
	//Generate, bind and upload data
	glGenBuffers(1, &bufferAddr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferAddr);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), &(data)[0], GL_STATIC_DRAW);
	++currentVAOIndex;

	GLenum error = glGetError();
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	return true;
}

bool StaticMesh::BufferUVDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr)
{
	GenVertexArrays(bufferAddr, vaoID, currentVAOIndex);
	//Generate, bind and upload data
	glEnableVertexAttribArray(2);
	glGenBuffers(1, &bufferAddr);
	glBindBuffer(GL_ARRAY_BUFFER, bufferAddr);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GL_FLOAT), &(data)[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	++currentVAOIndex;

	GLenum error = glGetError();
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	return true;
}

bool StaticMesh::BufferTextureDataToGPU(Bitmap* data, GLuint &bufferAddr)
{
	GLenum bitmapFormat;

	//TODO - maybe handle more of these formats?
	if (data->format() == Bitmap::Format_RGB)
	{
		bitmapFormat = GL_RGB;
	}
	else
	{
		bitmapFormat = GL_RGBA;
	}

	GenVertexArrays(bufferAddr, vaoID, currentVAOIndex);
	//Generate, bind and upload data
	glGenTextures(1, &bufferAddr);
	glBindTexture(GL_TEXTURE_2D, bufferAddr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		bitmapFormat,
		(GLsizei)data->width(),
		(GLsizei)data->height(),
		0,
		bitmapFormat,
		GL_UNSIGNED_BYTE,
		data->pixelBuffer());
	//Reset the texture
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	++currentVAOIndex;

	GLenum error = glGetError();
	glBindVertexArray(0);
	return true;
}

bool StaticMesh::GenVertexArrays(GLuint bufferAddr, GLuint &arrayAddr, int &currentIndex)
{
	if (!VAOGenerated)
	{
		glGenVertexArrays(1, &arrayAddr);
		VAOGenerated = true;
	}
	glBindVertexArray(arrayAddr);



	return true;
}


const GLuint StaticMesh::GetVerticesID()
{
	return verticesID;
}


const GLuint StaticMesh::GetNormalsID()
{
	return normalsID;
}

const GLuint StaticMesh::GetVAO()
{
	return vaoID;
}

const GLuint StaticMesh::GetTextureID()
{
	return textureID;
}


const std::vector<GLfloat> StaticMesh::GetVertices()
{
	return vertices;
}


const std::vector<GLfloat> StaticMesh::GetNormals()
{
	return normals;
}

const std::vector<GLfloat> StaticMesh::GetUVs()
{
	return UVs;
}

const std::vector<GLuint> StaticMesh::GetIndices()
{
	return indices;
}

const unsigned int StaticMesh::GetIndicesCount()
{
	return indicesCount;
}
const unsigned int StaticMesh::GetVerticesCount()
{
	return verticesCount;
}



