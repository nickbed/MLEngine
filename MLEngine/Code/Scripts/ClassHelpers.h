#ifndef CLASSHELPERS_H
#define CLASSHELPERS_H

#include <GLM/glm.hpp>
#include "..\Resources\GPUResourceManager.h"
#include "..\Resources\ResourceManager.h"
#include "..\Resources\OBJModel.h"
#include "..\Components\StaticMesh.h"
#include "../Scenes/SceneManager.h"

#include "../Components/BasicKeyMovement.h"
#include "../Components/Script.h"
#include "../Components/StaticMesh.h"

#include "../Interfaces/IComponent.h"
#include "../Interfaces/IEntity.h"

//Proxy classes for GLM
struct Vec3Helper
{
	static glm::vec3* _new(double x, double y, double z)
	{
		glm::vec3* v = new glm::vec3(x, y, z);
		return v;
	}

	template <unsigned index>
	static float get(glm::vec3 const* vec)
	{
		if (index == 0)
			return vec->x;
		else
			if (index == 1)
				return vec->y;
			else
				if (index == 2)
					return vec->z;

	}

	template <unsigned index>
	static void set(glm::vec3* vec, float value)
	{
		if (index == 0)
			vec->x = value;
		else
			if (index == 1)
				vec->y = value;
			else
				if (index == 2)
					vec->z = value;
	}
};



struct StaticMeshHelper
{
	static float getTexture(StaticMesh const* mesh)
	{
		return -1.f;
	}

	static void setTexture(StaticMesh* mesh, std::string path)
	{
		ImageTexture* gotTexture = mauvefileresource::ResourceManager::GetResource<ImageTexture>(path);
		if (gotTexture == nullptr)
		{
			//Use the no texture found texture
			gotTexture = mauvefileresource::ResourceManager::GetResource<ImageTexture>("data\\images\\default.png");
		}
		GPUTexture* gotGPUTexture = mauvegpuresource::GPUResourceManager::GetResource<GPUTexture>(path);
		if (gotGPUTexture == nullptr)
		{
			gotGPUTexture = new GPUTexture();
			gotGPUTexture->SetTexture(gotTexture);
			gotGPUTexture = mauvegpuresource::GPUResourceManager::LoadResource<GPUTexture>(gotGPUTexture, path);
		}
		mesh->SetTexture(*gotGPUTexture);
	}

	static float getModel(StaticMesh const* mesh)
	{
		return -1.f;
	}

	static void setModel(StaticMesh* mesh, std::string path)
	{
		OBJModel* gotModel = mauvefileresource::ResourceManager::GetResource<OBJModel>(path);
		if (gotModel != nullptr)
		{
			//Put the obj data in the thing
			GPUModel* gotGPUModel = mauvegpuresource::GPUResourceManager::GetResource<GPUModel>(path);
			if (gotGPUModel == nullptr)
			{
				//Create and load in new gpu model
				gotGPUModel = new GPUModel();
				gotGPUModel->SetAllData(gotModel->GetVertices(), gotModel->GetVertexCount(), gotModel->GetNormals(), gotModel->GetNormalCount(), gotModel->GetUVs(), gotModel->GetUVCount(), gotModel->GetIndicies(), gotModel->GetIndexCount());
				gotGPUModel = mauvegpuresource::GPUResourceManager::LoadResource(gotGPUModel, path);
			}
			mesh->SetModel(*gotGPUModel);
		}
	}

	static StaticMesh* _new(std::string id)
	{
		StaticMesh* mesh = new StaticMesh(id);
		return mesh;
	}
};


struct BoundingVolumesHelper
{
	static BoundingVolume* _newBase(std::string id)
	{
		BoundingVolume* volume = new BoundingVolume(id);
		return volume;
	}
	static BoundingSphere* _newSphere(std::string id)
	{
		BoundingSphere* volume = new BoundingSphere(id);
		return volume;
	}
	static BoundingCapsule* _newCapsule(std::string id)
	{
		BoundingCapsule* volume = new BoundingCapsule(id);
		return volume;
	}
	static BoundingBox* _newBox(std::string id)
	{
		BoundingBox* volume = new BoundingBox(id);
		return volume;
	}
	static BoundingBoxO* _newBoxO(std::string id)
	{
		BoundingBoxO* volume = new BoundingBoxO(id);
		return volume;
	}

};

struct IEntityHelper
{
	static ComponentManager* getComponentManager(IEntity const* ent)
	{
		return ent->Components;
	}
	static void setComponentManager(IEntity* ent, ComponentManager* componentmanager)
	{
		delete ent->Components;
		ent->Components = componentmanager;
	}
};
#endif