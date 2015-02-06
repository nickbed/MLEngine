#include "CollisionSystem.h"

std::vector<IEntity*> CollisionSystem::dynamics;
std::vector<IEntity*> CollisionSystem::statics;

void CollisionSystem::Init()
{
}

bool CollisionSystem::Update(float dt)
{
	return true;
}

void CollisionSystem::Destroy()
{
}

void CollisionSystem::AddStaticEntity(IEntity* entity)
{
	NULLPTRCHECK(entity,"Entity is a null pointer");
	statics.push_back(entity);
}

void CollisionSystem::AddDynamicEntity(IEntity* entity)
{
	NULLPTRCHECK(entity,"Entity is a null pointer");
	dynamics.push_back(entity);
}

void CollisionSystem::CheckCollisions()
{
	if(dynamics.size()>0)
	{
		for(auto entityi = dynamics.begin(); entityi != dynamics.end()-1; ++entityi)
		{
			for(auto entityj = entityi+1; entityj != dynamics.end(); ++entityj)
			{
				if(EntitiesCollided(*entityi,*entityj))
				{
					std::cout << "Collision message spam activate!!!" << std::endl;
				}
			}
			for(auto entityj = statics.begin(); entityj != statics.end(); ++entityj)
			{
				if(EntitiesCollided(*entityi,*entityj))
				{
					std::cout << "Collision message spam activate!!!" << std::endl;
				}
			}
		}
	}
}

bool CollisionSystem::EntitiesCollided(IEntity* entitya, IEntity* entityb)
{
	NULLPTRCHECK(entitya,"entitya passed into EntitiesCollided is a null pointer");
	NULLPTRCHECK(entityb,"entityb passed into EntitiesCollided is a null pointer");
	BoundingVolume* volumea = static_cast<BoundingVolume*>(entitya->Components->GetComponentsOfType("boundingbox").at(0));
	BoundingVolume* volumeb = static_cast<BoundingVolume*>(entityb->Components->GetComponentsOfType("boundingbox").at(0));
	BoundingBox* boxa = static_cast<BoundingBox*>(volumea);
	BoundingBox* boxb = static_cast<BoundingBox*>(volumeb);
	
	glm::vec3 amin = boxa->GetLeftBotFore() + entitya->Transform->GetPosition();
	glm::vec3 amax = boxa->GetRightTopRear() + entitya->Transform->GetPosition();
	glm::vec3 bmin = boxb->GetLeftBotFore() + entityb->Transform->GetPosition();
	glm::vec3 bmax = boxb->GetRightTopRear() + entityb->Transform->GetPosition();
	
	return(amax.x >= bmin.x &&
		amin.x <= bmax.x &&
		amax.y >= bmin.y &&
		amin.y <= bmax.y &&
		amax.z >= bmin.z &&
		amin.z <= bmax.z);
}