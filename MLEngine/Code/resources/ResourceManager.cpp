#include "ResourceManager.h"

namespace mauvefileresource
{
	std::unique_ptr<ResourceMap> ResourceManager::resources = std::unique_ptr<ResourceMap>(new ResourceMap);

	ResourceManager::~ResourceManager()
	{
		UnloadAllResources();
		//Kill unique ptr
		resources.release();
	}

	bool ResourceManager::ReserveMapSpace()
	{
		//Initially resize our map
		if (resources->size() == 0)
		{
			resources->reserve(1024);
			DEBUGWRITEINFO("Reserved 1024 spaces for resources","");
		}
		return true;
	}

	template <>
	Shader* ResourceManager::LoadResource<Shader>(std::string resourcePath)
	{
		ReserveMapSpace();
		//Does it already exist?
		IResource* gotResource = nullptr;
		if(resources->find(resourcePath) == resources->end())
		{
			//Put it into the map
			gotResource = new Shader(resourcePath);
			if (gotResource->HasLoaded()) DEBUGWRITEINFO("Successfully loaded shader:", resourcePath)
			else
			{
				return nullptr;
			}
			resources->insert(std::pair<std::string, IResource*>(resourcePath, gotResource));
			
		}
		else
		{
			gotResource = resources->find(resourcePath)->second;
		}
		return (Shader*)gotResource;
	}

	template <>
	Shader* ResourceManager::GetResource<Shader>(std::string resourcePath)
	{
		if(resources->find(resourcePath) == resources->end())
		{
			return LoadResource<Shader>(resourcePath);
		}
		IResource* gotResource = resources->find(resourcePath)->second;
		return (Shader*)gotResource;
	}

	template <>
	OBJModel* ResourceManager::LoadResource<OBJModel>(std::string resourcePath)
	{
		ReserveMapSpace();
		//Does it already exist?
		IResource* gotResource = nullptr;
		if (resources->find(resourcePath) == resources->end())
		{
			//Put it into the map
			gotResource = new OBJModel(resourcePath);
			if (gotResource->HasLoaded()) DEBUGWRITEINFO("Successfully loaded OBJ Model:", resourcePath)
			else
			{
				return nullptr;
			}
			resources->insert(std::pair<std::string, IResource*>(resourcePath, gotResource));

		}
		else
		{
			gotResource = resources->find(resourcePath)->second;
		}
		return (OBJModel*)gotResource;
	}

	template <>
	OBJModel* ResourceManager::GetResource<OBJModel>(std::string resourcePath)
	{
		if (resources->find(resourcePath) == resources->end())
		{
			return LoadResource<OBJModel>(resourcePath);
		}
		IResource* gotResource = resources->find(resourcePath)->second;
		return (OBJModel*)gotResource;
	}

	template <>
	JSONFile* ResourceManager::LoadResource<JSONFile>(std::string resourcePath)
	{
		ReserveMapSpace();
		//Does it already exist?
		IResource* gotResource = nullptr;
		if (resources->find(resourcePath) == resources->end())
		{
			//Put it into the map
			gotResource = new JSONFile(resourcePath);
			if (gotResource->HasLoaded()) DEBUGWRITEINFO("Successfully loaded JSON File:", resourcePath)
			else
			{
				return nullptr;
			}
			//resources->insert(std::pair<std::string, IResource*>(resourcePath, gotResource));

		}
		else
		{
			gotResource = resources->find(resourcePath)->second;
		}
		return (JSONFile*)gotResource;
	}

	template <>
	JSONFile* ResourceManager::GetResource<JSONFile>(std::string resourcePath)
	{
		if (resources->find(resourcePath) == resources->end())
		{
			return LoadResource<JSONFile>(resourcePath);
		}
		IResource* gotResource = resources->find(resourcePath)->second;
		return (JSONFile*)gotResource;
	}

	template <>
	ImageTexture* ResourceManager::LoadResource<ImageTexture>(std::string resourcePath)
	{
		ReserveMapSpace();
		//Does it already exist?
		IResource* gotResource = nullptr;
		if (resources->find(resourcePath) == resources->end())
		{
			//Put it into the map
			gotResource = new ImageTexture(resourcePath);
			if (gotResource->HasLoaded()) DEBUGWRITEINFO("Successfully loaded Image File:", resourcePath)
			else
			{
				return nullptr;
			}
			resources->insert(std::pair<std::string, IResource*>(resourcePath, gotResource));

		}
		else
		{
			gotResource = resources->find(resourcePath)->second;
		}
		return (ImageTexture*)gotResource;
	}

	template <>
	ImageTexture* ResourceManager::GetResource<ImageTexture>(std::string resourcePath)
	{
		if (resources->find(resourcePath) == resources->end())
		{
			return LoadResource<ImageTexture>(resourcePath);
		}
		IResource* gotResource = resources->find(resourcePath)->second;
		return (ImageTexture*)gotResource;
	}

	bool ResourceManager::UnloadAllResources()
	{
		DEBUGWRITEINFO("Number of resources unloading:",resources->size());
		for ( auto it = resources->begin(); it != resources->end(); ++it )
		{
			delete it->second;
		}
		//Get rid of useless pointers
		resources->clear();
		return true;
	}

}