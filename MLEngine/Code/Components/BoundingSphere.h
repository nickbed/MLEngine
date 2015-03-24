#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H

#include "BoundingVolume.h"
#include "..\Assert\Assert.h"
#include "..\Interfaces\IComponent.h"
#include "..\..\glm\GLM\glm.hpp"

class BoundingSphere: public BoundingVolume
{
public:
	BoundingSphere(std::string id);
	BoundingSphere(std::string id, glm::vec3 center, float radius, bool isStatic);
	virtual ~BoundingSphere();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	
	glm::vec3 GetCenter();
	float GetRadius();
	
	void SetCenter(glm::vec3 cen);
	void SetRadius(float rad);

private:
	glm::vec3 center;
	float radius;
};

#endif