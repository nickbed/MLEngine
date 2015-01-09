#include "BasicBone.h"

BasicBone::BasicBone(std::string id) : IComponent(id), BoneTransform("bone")
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

BasicBone::~BasicBone()
{
	IComponent::~IComponent();
}

void BasicBone::Init()
{
}

void BasicBone::Update(float dt)
{
}

void BasicBone::Destroy()
{
}

bool BasicBone::UploadVertices(std::vector<GLfloat> verts)
{
	bool result = BufferDataToGPU(verts, verticesID, 0);
	if (result)
	{
		verticesCount = verts.size();
		vertices = verts;
	}

	return result;
}


bool BasicBone::UploadNormals(std::vector<GLfloat> norm)
{
	bool result = BufferDataToGPU(norm, normalsID, 1);
	if (result) normals = norm;
	return result;
}

bool BasicBone::UploadUVs(std::vector<GLfloat> uvs)
{
	bool result = BufferUVDataToGPU(uvs, uvID);
	if (result)
	{
		UVs = uvs;
	}
	return result;
}

bool BasicBone::UploadIndices(std::vector<GLuint> ind)
{
	bool result = BufferDataToGPU(ind, verticesID);
	if (result)
	{
		indicesCount = ind.size();
		indices = ind;
	}
	return result;
}

bool BasicBone::UploadTexture(Bitmap* texture)
{
	NULLPTRCHECK(texture, "Null pointer passed for texture bitmap");
	bool result = BufferTextureDataToGPU(texture, textureID);
	return result;
}

bool BasicBone::BufferDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr, GLuint currentLoc)
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

bool BasicBone::BufferDataToGPU(std::vector<GLuint> data, GLuint &bufferAddr)
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

bool BasicBone::BufferUVDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr)
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

bool BasicBone::BufferTextureDataToGPU(Bitmap* data, GLuint &bufferAddr)
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
	glBindTexture(GL_TEXTURE_2D, 0);
	++currentVAOIndex;

	GLenum error = glGetError();
	glBindVertexArray(0);
	return true;
}

bool BasicBone::GenVertexArrays(GLuint bufferAddr, GLuint &arrayAddr, int &currentIndex)
{
	if (!VAOGenerated)
	{
		glGenVertexArrays(1, &arrayAddr);
		VAOGenerated = true;
	}
	glBindVertexArray(arrayAddr);



	return true;
}


const GLuint BasicBone::GetVerticesID()
{
	return verticesID;
}


const GLuint BasicBone::GetNormalsID()
{
	return normalsID;
}

const GLuint BasicBone::GetVAO()
{
	return vaoID;
}

const GLuint BasicBone::GetTextureID()
{
	return textureID;
}


const std::vector<GLfloat> BasicBone::GetVertices()
{
	return vertices;
}


const std::vector<GLfloat> BasicBone::GetNormals()
{
	return normals;
}

const std::vector<GLfloat> BasicBone::GetUVs()
{
	return UVs;
}

const std::vector<GLuint> BasicBone::GetIndices()
{
	return indices;
}

const unsigned int BasicBone::GetIndicesCount()
{
	return indicesCount;
}
const unsigned int BasicBone::GetVerticesCount()
{
	return verticesCount;
}



