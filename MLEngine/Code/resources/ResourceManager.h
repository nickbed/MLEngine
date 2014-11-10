#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "../Assert/Assert.h"
#include "..\Interfaces\IResource.h"
#include <unordered_map>
#include <memory>
#include <string>
#include "Shader.h"

typedef std::unordered_map<std::string, IResource*> ResourceMap;

namespace mauveresource
{
	
	class ResourceManager
	{
	public:
		~ResourceManager();

		//For preloading
		template <class IResource>
		static IResource* LoadResource(std::string resourcePath);
		
		template <class IResource>
		//Lazy loading or already loaded
		static IResource* GetResource(std::string resourcePath); 

		static bool UnloadAllResources();

	private:
		static bool ReserveMapSpace();
		static std::unique_ptr<ResourceMap> resources;
	};

}
#endif