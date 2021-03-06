#include "BoundingVolume.h"

BoundingVolume::BoundingVolume(std::string id) : IComponent(id)
{
	Rigid_mass = 1.f;
	Rigid_vel = glm::vec3(0.f);
	Rigid_ang = glm::vec3(0.f);
	Rigid_density = 1.f;
	Rigid_friction = 1.f;
}

int BoundingVolume::GetType()
{
	return type;
}

bool BoundingVolume::IsStatic()
{
	return isStatic;
}

bool BoundingVolume::IsCollided()
{
	return collided;
}

void BoundingVolume::SetCollided(bool hasCollided)
{
	collided = hasCollided;
}

std::string BoundingVolume::GetOwnerId()
{
	return owner;
}

void BoundingVolume::Init()
{
}

void BoundingVolume::Update(float dt)
{
}

void BoundingVolume::Destroy()
{
}

BoundingVolume::~BoundingVolume()
{
	IComponent::~IComponent();
}