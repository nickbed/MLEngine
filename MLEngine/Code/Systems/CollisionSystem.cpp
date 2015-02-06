#include "CollisionSystem.h"

std::vector<BoundingVolume*> CollisionSystem::dynamics;
std::vector<BoundingVolume*> CollisionSystem::statics;

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

void CollisionSystem::AddStaticVolume(BoundingVolume* volume)
{
	NULLPTRCHECK(volume,"Static Volume is a null pointer");
	statics.push_back(volume);
}

void CollisionSystem::AddDynamicVolume(BoundingVolume* volume)
{
	NULLPTRCHECK(volume,"Dynamic Volume is a null pointer");
	dynamics.push_back(volume);
}

void CollisionSystem::CheckCollisions()
{
	if(dynamics.size()>0)
	{
		for(auto volumei = dynamics.begin(); volumei != dynamics.end()-1; ++volumei)
		{
			for(auto volumej = volumei+1; volumej != dynamics.end(); ++volumej)
			{
				if(HasCollided(*volumei,*volumej))
				{
					std::cout << "Collision message spam activate!!!" << std::endl;
				}
			}
			for(auto volumej = statics.begin(); volumej != statics.end(); ++volumej)
			{
				if(HasCollided(*volumei,*volumej))
				{
					std::cout << "Collision message spam activate!!!" << std::endl;
				}
			}
		}
	}
}

bool CollisionSystem::HasCollided(BoundingVolume* volumea, BoundingVolume* volumeb)
{
	NULLPTRCHECK(volumea,"entitya passed into EntitiesCollided is a null pointer");
	NULLPTRCHECK(volumeb,"entityb passed into EntitiesCollided is a null pointer");

	BoundingBox* boxa = static_cast<BoundingBox*>(volumea);
	BoundingBox* boxb = static_cast<BoundingBox*>(volumeb);
	
	glm::vec3 amin = boxa->GetLeftBotFore() +  volumea->getTransform()->GetPosition();
	glm::vec3 amax = boxa->GetRightTopRear() + volumea->getTransform()->GetPosition();
	glm::vec3 bmin = boxb->GetLeftBotFore() +  volumeb->getTransform()->GetPosition();
	glm::vec3 bmax = boxb->GetRightTopRear() + volumeb->getTransform()->GetPosition();
	
	return(amax.x >= bmin.x &&
		amin.x <= bmax.x &&
		amax.y >= bmin.y &&
		amin.y <= bmax.y &&
		amax.z >= bmin.z &&
		amin.z <= bmax.z);
}