#ifndef BOUNDINGBOXO_H
#define BOUNDINGBOXO_H

#include "BoundingVolume.h"
#include "..\Assert\Assert.h"
#include "..\Interfaces\IComponent.h"
#include "..\..\glm\GLM\glm.hpp"

class BoundingBoxO: public BoundingVolume
{
public:
	BoundingBoxO(std::string id);
	BoundingBoxO(std::string id, glm::vec3 center, glm::vec3 extent, bool isStatic, std::string owner);
	virtual ~BoundingBoxO();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	
	glm::vec3 GetCenter();
	glm::vec3 GetExtent();
	
	void SetCenter(glm::vec3 cen);
	void SetExtent(glm::vec3 ext);

private:
	glm::vec3 center;
	glm::vec3 extent;
};

#endif