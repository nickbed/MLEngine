#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(std::string id) : BoundingVolume(id)
{
	type = BOUNDING_TYPE_SPHERE;
	center = glm::vec3(0.0);
	radius = 1.f;
	isStatic = true;
}

BoundingSphere::BoundingSphere(std::string id, glm::vec3 center, float radius, bool isStatic) : BoundingVolume(id)
{
	type = BOUNDING_TYPE_SPHERE;
	this->center = center;
	this->radius = radius;
	this->isStatic = isStatic;
}

void BoundingSphere::SetCenter(glm::vec3 cen)
{
	center = cen;
}

void BoundingSphere::SetRadius(float rad)
{
	radius = rad;
}

glm::vec3 BoundingSphere::GetCenter()
{
	return center;
}

float BoundingSphere::GetRadius()
{
	return radius;
}

void BoundingSphere::Init()
{
}

void BoundingSphere::Update(float dt)
{
}

void BoundingSphere::Destroy()
{
}

BoundingSphere::~BoundingSphere()
{
	IComponent::~IComponent();
}