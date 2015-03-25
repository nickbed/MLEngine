#include "BoundingBoxO.h"

BoundingBoxO::BoundingBoxO(std::string id) : BoundingVolume(id)
{
	type = BOUNDING_TYPE_OBBOX;
	center = glm::vec3(0.0);
	extent = glm::vec3(1.0);
	isStatic = true;
	Rigid_vel = glm::vec3(0.f);
	Rigid_ang = glm::vec3(0.f);
}

BoundingBoxO::BoundingBoxO(std::string id, glm::vec3 center, glm::vec3 extend, bool isStatic) : BoundingVolume(id)
{
	type = BOUNDING_TYPE_OBBOX;
	this->center = center;
	this->extent = extend;
	this->isStatic = isStatic;
	Rigid_vel = glm::vec3(0.f);
	Rigid_ang = glm::vec3(0.f);
}

void BoundingBoxO::SetCenter(glm::vec3 cen)
{
	center = cen;
}

void BoundingBoxO::SetExtent(glm::vec3 ext)
{
	extent = ext;
}

glm::vec3 BoundingBoxO::GetCenter()
{
	return center;
}

glm::vec3 BoundingBoxO::GetExtent()
{
	return extent;
}


void BoundingBoxO::Init()
{
}

void BoundingBoxO::Update(float dt)
{
}

void BoundingBoxO::Destroy()
{
}

BoundingBoxO::~BoundingBoxO()
{
	IComponent::~IComponent();
}