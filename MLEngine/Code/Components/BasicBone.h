#ifndef BasicBoneCOMPONENT_H
#define BasicBoneCOMPONENT_H
#include "..\Assert\Assert.h"
#include "..\Interfaces\IComponent.h"
#include "..\..\Libs\TextureLoader\Bitmap.h"
#include "..\Components\Transform.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"


#include <string>
#include <vector>

class BasicBone : public IComponent
{
public:
	BasicBone(std::string id);
	virtual ~BasicBone();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();

	bool UploadVertices(std::vector<GLfloat> vertices);
	bool UploadNormals(std::vector<GLfloat> normals);
	bool UploadUVs(std::vector<GLfloat> UVs);
	bool UploadIndices(std::vector<GLuint> indices);
	bool UploadTexture(Bitmap* texture);

	bool BufferDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr, GLuint currentLoc);
	bool BufferDataToGPU(std::vector<GLuint> data, GLuint &bufferAddr);
	bool BufferUVDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr);
	bool BufferTextureDataToGPU(Bitmap* data, GLuint &bufferAddr);
	bool GenVertexArrays(GLuint bufferAddr, GLuint &arrayAddr, int &currentIndex);

	const GLuint GetVerticesID();
	const GLuint GetNormalsID();
	const GLuint GetUVsID();
	const GLuint GetIndicesID();
	const GLuint GetVAO();
	const GLuint GetTextureID();

	const std::vector<GLfloat> GetVertices();
	const std::vector<GLfloat> GetNormals();
	const std::vector<GLfloat> GetUVs();
	const std::vector<GLuint> GetIndices();

	const unsigned int GetVerticesCount();
	const unsigned int GetIndicesCount();
	TransformComponent BoneTransform; 

private:
	GLuint verticesID;
	GLuint normalsID;
	GLuint ID;
	GLuint vaoID;
	GLuint uvID;
	GLuint indicesID;
	GLuint textureID;

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> UVs;
	std::vector<GLuint> indices;
	bool VAOGenerated;
	int currentVAOIndex;
	unsigned int indicesCount;
	unsigned int verticesCount;


};

#endif