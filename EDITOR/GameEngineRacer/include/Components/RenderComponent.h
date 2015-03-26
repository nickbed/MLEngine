#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include <Windows.h>
#include <iostream>

#include <glew.h>
#include <glfw3.h>
#include "3rdParty\Texture.h"
#include "ResourceManager.h"
#include <glew.h>
#include <glfw3.h>
#include <vector>
#include "GameObjects\GameObject.h"

class GameObject;
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
	void init( Model* model, Texture* nTexture);
	void setTexture(Texture* nTexture){texture = nTexture;};
	void loadTexture(GameObject* g );
	//void loadModel();
	//void setModel(const Model& model){m_model = model;};
	void update();
};
#endif