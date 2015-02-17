#include "BoundingVolume.h"

BoundingVolume::BoundingVolume(std::string id) : IComponent(id)
{
}

int BoundingVolume::getType()
{
	return type;
}

bool BoundingVolume::IsStatic()
{
	return isStatic;
}

TransformComponent* BoundingVolume::getTransform()
{
	return transform;
}

void BoundingVolume::setTransform(TransformComponent* trans)
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
}