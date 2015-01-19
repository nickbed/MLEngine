#include "StaticMesh.h"

StaticMesh::StaticMesh(std::string id) : IComponent(id)
{
}

StaticMesh::~StaticMesh()
{
	IComponent::~IComponent();
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


void StaticMesh::SetModel(GPUModel mod)
{
	model = mod;
	modelVAOID = model.GetDataLocation();
	modelIndicesCount = model.GetIndicesCount();
}


void StaticMesh::SetTexture(GPUTexture tex)
{
	texture = tex;
	textureID = texture.GetDataLocation();
}

const GLuint StaticMesh::GetModelVAOID()
{
	return modelVAOID;
}

const GLuint StaticMesh::GetModelIndicesCount()
{
	return modelIndicesCount;
}


const GLuint StaticMesh::GetTextureID()
{
	return textureID;
}

const GPUModel StaticMesh::GetModel()
{
	return model;
}

const GPUTexture StaticMesh::GetTexture()
{
	return texture;
}








