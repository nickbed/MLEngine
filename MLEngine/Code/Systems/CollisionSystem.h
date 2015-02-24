#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include "GLM\gtx\transform.hpp"
#include "..\Interfaces\IEntity.h"
#include "..\Components\BoundingVolume.h"
#include "..\Components\BoundingBox.h"
#include "..\Components\BoundingBoxO.h"
#include "..\Components\BoundingSphere.h"
#include "..\Components\BoundingCapsule.h"

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
	static bool CheckVolumes(BoundingVolume* volumea, BoundingVolume* volumeb);
	
private:
	static std::vector<BoundingVolume*> statics;
	static std::vector<BoundingVolume*> dynamics;

	static bool HasCollided(BoundingBox* boxa, BoundingBox* boxb);
	static bool HasCollided(BoundingBoxO* boxa, BoundingBoxO* boxb);
	static bool HasCollided(BoundingBoxO* box, BoundingCapsule* capsule);
};

#endif