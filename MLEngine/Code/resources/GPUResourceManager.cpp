#include "GPUResourceManager.h"

namespace mauvegpuresource
{
	std::unique_ptr<GPUResourceMap> GPUResourceManager::gpuresources = std::unique_ptr<GPUResourceMap>(new GPUResourceMap);


	GPUResourceManager::~GPUResourceManager()
	{
		UnloadAllResources();
		//Kill unique ptr
		gpuresources.release();
	}

	bool GPUResourceManager::UnloadAllResources()
	{
		DEBUGWRITEINFO("Number of GPU resources unloading:", gpuresources->size());
		for (auto it = gpuresources->begin(); it != gpuresources->end(); ++it)
		{
			it->second->DeleteData();
			delete it->second;
		}
		//Get rid of useless pointers
		gpuresources->clear();
		return true;
	}

	bool GPUResourceManager::ReserveMapSpace()
	{
		//Initially resize our map
		if (gpuresources->size() == 0)
		{
			gpuresources->reserve(1024);
			DEBUGWRITEINFO("Reserved 1024 spaces for GPU resources", "");
		}
		return true;
	}

	template <>
	GPUModel* GPUResourceManager::LoadResource<GPUModel>(GPUModel* model, std::string name)
	{
		ReserveMapSpace();
		//Does it already exist?
		IGPUResource* gotResource = nullptr;
		if (gpuresources->find(name) == gpuresources->end())
		{
			//Put it into the map
			if (model->UploadData()) DEBUGWRITEINFO("Successfully loaded GPU Model:", name)
			else
			{
				return nullptr;
			}
			gotResource = (IGPUResource*)model;
			gpuresources->insert(std::pair<std::string, IGPUResource*>(name, gotResource));
		}
		else
		{
			gotResource = gpuresources->find(name)->second;
		}
		model->UploadData();
		return model;
	}

	template <>
	GPUModel* GPUResourceManager::GetResource<GPUModel>(std::string name)
	{
		if (gpuresources->find(name) == gpuresources->end())
		{
			return nullptr;
		}
		IGPUResource* gotResource = gpuresources->find(name)->second;
		return (GPUModel*)gotResource;
		return nullptr;
	}

	template <>
	GPUTexture* GPUResourceManager::LoadResource<GPUTexture>(GPUTexture* model, std::string name)
	{
		ReserveMapSpace();
		//Does it already exist?
		IGPUResource* gotResource = nullptr;
		if (gpuresources->find(name) == gpuresources->end())
		{
			//Put it into the map
			if (model->UploadData()) DEBUGWRITEINFO("Successfully loaded GPU Texture:", name)
			else
			{
				return nullptr;
			}
			gotResource = (IGPUResource*)model;
			gpuresources->insert(std::pair<std::string, IGPUResource*>(name, gotResource));
		}
		else
		{
			gotResource = gpuresources->find(name)->second;
		}
		return (GPUTexture*)gotResource;
	}

	template <>
	GPUTexture* GPUResourceManager::GetResource<GPUTexture>(std::string name)
	{
		if (gpuresources->find(name) == gpuresources->end())
		{
			return nullptr;
		}
		IGPUResource* gotResource = gpuresources->find(name)->second;
		return (GPUTexture*)gotResource;
	}

	
}

