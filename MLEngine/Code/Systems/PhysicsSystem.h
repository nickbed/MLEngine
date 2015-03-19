#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H
#include "..\Messages\CollisionMessage.h"
#include "..\Messages\MessageManager.h"
#include "..\Interfaces\IEntity.h"
#include "..\Components\BoundingVolume.h"
#include "GLFW\glfw3.h"
#include "GLM\gtx\transform.hpp"

class PhysicsSystem : public IEntity
{
public:

	DEFAULTENTITYCONSTRUCTOR(PhysicsSystem);

	void Init();

	bool Update(float dt);

	void Destroy();

	static void msg_HandleCollision(mauvemessage::BaseMessage* msg);
};

#endif