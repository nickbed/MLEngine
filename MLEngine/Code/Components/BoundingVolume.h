#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include "..\Assert\Assert.h"
#include "..\Interfaces\IComponent.h"
#include "..\..\glm\GLM\glm.hpp"

class BoundingVolume: public IComponent
{
public:
	BoundingVolume(std::string id);
	virtual ~BoundingVolume();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();

	void SetStatic(bool isStatic);
	bool IsStatic();

protected:
	bool isStatic;
};
#endif