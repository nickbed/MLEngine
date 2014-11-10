#ifndef THREEDGRAPHICSCOMPONENT_H
#define THREEDGRAPHICSCOMPONENT_H
#include "..\Interfaces\IComponent.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"


#include <string>
#include <vector>

class ThreeDGraphics : public IComponent
{
public:
	ThreeDGraphics(std::string id);
	virtual ~ThreeDGraphics();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();

	bool UploadVertices(std::vector<GLfloat> vertices);
	bool UploadColours(std::vector<GLfloat> colours);
	bool BufferDataToGPU(std::vector<GLfloat> data, GLuint &bufferAddr);
	bool GenVertexArrays(GLuint bufferAddr, GLuint &arrayAddr);

	const GLuint GetVerticesID();
	const GLuint GetColoursID();
	//const GLuint GetVAO();
	const std::vector<GLfloat> GetVertices();
	const std::vector<GLfloat> GetColours();
	//TODO - Normals/texture support

private:
	GLuint verticesID;
	GLuint coloursID;
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> colours;

};

#endif