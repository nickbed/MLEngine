#include "BoundingVolume.h"

BoundingVolume::BoundingVolume(std::string id) : IComponent(id)
{
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

TransformComponent* BoundingVolume::GetTransform()
{
	return transform;
}

void BoundingVolume::SetTransform(TransformComponent* trans)
{
	transform = trans;
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