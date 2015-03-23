#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include <vector>
#include "GLM\gtx\transform.hpp"
#include "..\Components\BoundingVolume.h"
#include "..\Components\BoundingBox.h"
#include "..\Components\BoundingBoxO.h"
#include "..\Components\BoundingSphere.h"
#include "..\Components\BoundingCapsule.h"
#include "..\Messages\CollisionMessage.h"
#include "..\Messages\MessageManager.h"


class CollisionSystem 
{
public:
	virtual ~CollisionSystem();
	void Init();

	bool Update(float dt);

	static void Destroy();

	static void AddStaticVolume(BoundingVolume* volume);
	static void AddDynamicVolume(BoundingVolume* volume);

	static void CheckCollisions();
	static CollisionManifold CheckVolumes(BoundingVolume* volumea, BoundingVolume* volumeb);

	
private:
	static std::vector<BoundingVolume*> statics;
	static std::vector<BoundingVolume*> dynamics;

	static CollisionManifold HasCollided(BoundingBox* boxa, BoundingBox* boxb);
	static CollisionManifold HasCollided(BoundingBoxO* boxa, BoundingBoxO* boxb);
	static CollisionManifold HasCollided(BoundingBoxO* box, BoundingCapsule* capsule);

};

#endif