#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H


#include <iostream>
#include "ShaderComponent.h"
#include "3rdParty\gl_core_4_3.hpp"
#include "glfw3.h"
#include "3rdParty\Texture.h"
#include "ResourceManager.h"

#include <vector>
class ResourceManager;
class RenderComponent
{
private:
	Model m_model;
	GLuint positionBufferHandle;
	GLuint normalBufferHandle;
	GLuint uvBufferHandle;
	ResourceManager* rManager;
	GLuint vboHandles[2];
	GLuint vaoHandle;
	GLsizei indicesCount;
	Texture* texture;
public:
	RenderComponent();
	void init( Model* model, Texture* nTexture, const std::string& shaderID);
	void update();
};
#endif