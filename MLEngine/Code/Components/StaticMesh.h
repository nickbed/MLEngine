#ifndef STATICMESHCOMPONENT_H
#define STATICMESHCOMPONENT_H
#include "..\Assert\Assert.h"
#include "..\Resources\GPUTexture.h"
#include "..\Interfaces\IComponent.h"
#include "..\..\Libs\TextureLoader\Bitmap.h"
#include "..\Resources\GPUModel.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"


#include <string>
#include <vector>

class StaticMesh : public IComponent
{
public:
	StaticMesh(std::string id);
	virtual ~StaticMesh();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	//TODO - Normals/texture support

	void SetModel(GPUModel model);
	void SetTexture(GPUTexture texture);

	const GLuint GetModelVAOID();
	const GLuint GetModelIndicesCount();
	const GLuint GetTextureID();

	const GPUModel GetModel();
	const GPUTexture GetTexture();


private:
	GPUModel model;
	GPUTexture texture;

	//NEEDS to be cached
	GLuint modelVAOID;
	GLuint modelIndicesCount;
	GLuint textureID;
};

#endif