#ifndef BOUNDINGCAPSULE_H
#define BOUNDINGCAPSULE_H

#include "BoundingVolume.h"
#include "..\Assert\Assert.h"
#include "..\Interfaces\IComponent.h"
#include "..\..\glm\GLM\glm.hpp"

class BoundingCapsule: public BoundingVolume
{
public:
	BoundingCapsule(std::string id);
	BoundingCapsule(std::string id, glm::vec3 center, float radius, float extent, bool isStatic);
	virtual ~BoundingCapsule();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	
	glm::vec3 GetCenter();
	float GetRadius();
	float GetExtent();

	void SetCenter(glm::vec3 cen);
	void SetRadius(float rad);
	void SetExtent(float ext);

private:
	glm::vec3 center;
	float extent;
	float radius;
};

#endif