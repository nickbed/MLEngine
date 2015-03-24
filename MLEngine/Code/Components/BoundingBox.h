#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "BoundingVolume.h"
#include "..\Assert\Assert.h"
#include "..\Interfaces\IComponent.h"
#include "..\..\glm\GLM\glm.hpp"

class BoundingBox: public BoundingVolume
{
public:
	BoundingBox(std::string id);
	BoundingBox(std::string id, glm::vec3 left, glm::vec3 right, bool isStatic);
	virtual ~BoundingBox();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	
	glm::vec3 GetLeftBotFore();
	glm::vec3 GetRightTopRear();
	
	void SetLeftBotFore(glm::vec3 left);
	void SetRightTopRear(glm::vec3 right);

private:
	glm::vec3 leftbotfore;
	glm::vec3 righttoprear;
};

#endif