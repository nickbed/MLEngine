#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include "..\Interfaces\IEntity.h"
#include "..\Components\BoundingVolume.h"
#include "..\Components\BoundingBox.h"

class CollisionSystem : public IEntity
{
public:

	DEFAULTENTITYCONSTRUCTOR(CollisionSystem);

	void Init();

	bool Update(float dt);

	void Destroy();

	static void AddStaticEntity(IEntity* entity);
	static void AddDynamicEntity(IEntity* entity);

	static void CheckCollisions();
	static bool EntitiesCollided(IEntity* entitya, IEntity* entityb);
	
private:
	static std::vector<IEntity*> statics;
	static std::vector<IEntity*> dynamics;
};

#endif