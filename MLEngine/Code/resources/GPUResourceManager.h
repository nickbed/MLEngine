#ifndef GPUResourceManager_H
#define GPUResourceManager_H
#include <unordered_map>
#include <memory>
#include "..\Assert\Assert.h"
#include "..\Resources\GPUModel.h"
#include "..\Resources\GPUTexture.h"
#include "..\Interfaces\IGPUResource.h"


typedef std::unordered_map<std::string, IGPUResource*> GPUResourceMap;

namespace mauvegpuresource
{

	class GPUResourceManager
	{
	public:
		~GPUResourceManager();
		//For preloading
		template <class IGPUResource>
		static IGPUResource* LoadResource(IGPUResource* resource, std::string name);

		template <class IGPUResource>
		//Lazy loading or already loaded
		static IGPUResource* GetResource(std::string name);

		static bool UnloadAllResources();

	private:
		static bool ReserveMapSpace();
		static std::unique_ptr<GPUResourceMap> gpuresources;
	};

}

#endif