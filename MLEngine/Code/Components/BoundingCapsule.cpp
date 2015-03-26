#include "BoundingCapsule.h"

BoundingCapsule::BoundingCapsule(std::string id) : BoundingVolume(id)
{
	type = BOUNDING_TYPE_CAPSULE;
	center = glm::vec3(0.0);
	extent = 1.0;
	isStatic = true;
}

BoundingCapsule::BoundingCapsule(std::string id, glm::vec3 center, float radius, float extent, bool isStatic,std::string owner) : BoundingVolume(id)
{
	type = BOUNDING_TYPE_CAPSULE;
	this->center = center;
	this->radius = radius;
	this->extent = extent;
	this->isStatic = isStatic;
	this->owner = owner;
}

void BoundingCapsule::SetCenter(glm::vec3 cen)
{
	center = cen;
}

void BoundingCapsule::SetExtent(float ext)
{
	extent = ext;
}

void BoundingCapsule::SetRadius(float rad)
{
	radius = rad;
}

glm::vec3 BoundingCapsule::GetCenter()
{
	return center;
}

float BoundingCapsule::GetExtent()
{
	return extent;
}

float BoundingCapsule::GetRadius()
{
	return radius;
}

void BoundingCapsule::Init()
{
}

void BoundingCapsule::Update(float dt)
{
}

void BoundingCapsule::Destroy()
{
}

BoundingCapsule::~BoundingCapsule()
{
	IComponent::~IComponent();
}