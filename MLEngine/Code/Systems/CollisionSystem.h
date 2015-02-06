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

	static void AddStaticVolume(BoundingVolume* volume);
	static void AddDynamicVolume(BoundingVolume* volume);

	static void CheckCollisions();
	static bool HasCollided(BoundingVolume* volumea, BoundingVolume* volumeb);
	
private:
	static std::vector<BoundingVolume*> statics;
	static std::vector<BoundingVolume*> dynamics;
};

#endif