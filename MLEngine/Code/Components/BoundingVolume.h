#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#define BOUNDING_TYPE_AABBOX 1
#define BOUNDING_TYPE_OBBOX 2
#define BOUNDING_TYPE_SPHERE 3
#define BOUNDING_TYPE_CYLINDER 4

#include "..\Assert\Assert.h"
#include "..\Interfaces\IComponent.h"
#include "..\..\glm\GLM\glm.hpp"
#include "..\Components\Transform.h"

class BoundingVolume: public IComponent
{
public:
	BoundingVolume(std::string id);
	virtual ~BoundingVolume();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	
	int getType();

	void SetStatic(bool isStatic);
	bool IsStatic();

	TransformComponent* getTransform();
	void setTransform(TransformComponent*);

protected:
	int type;
	bool isStatic;
	TransformComponent* transform;
};
#endif