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
#endif;


