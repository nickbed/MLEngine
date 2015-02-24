#include "BoundingBox.h"

BoundingBox::BoundingBox(std::string id) : BoundingVolume(id)
{
	type = BOUNDING_TYPE_AABBOX;
	leftbotfore = glm::vec3(0.0);
	righttoprear = glm::vec3(0.0);
	isStatic = true;
}

BoundingBox::BoundingBox(std::string id, glm::vec3 left, glm::vec3 right, bool isStatic) : BoundingVolume(id)
{
	type = BOUNDING_TYPE_AABBOX;
	leftbotfore = left;
	righttoprear = right;
	this->isStatic = isStatic;
}

void BoundingBox::SetLeftBotFore(glm::vec3 left)
{
	leftbotfore = left;
}

void BoundingBox::SetRightTopRear(glm::vec3 right)
{
	righttoprear = right;
}

glm::vec3 BoundingBox::GetLeftBotFore()
{
	return leftbotfore;
}

glm::vec3 BoundingBox::GetRightTopRear()
{
	return righttoprear;
}


void BoundingBox::Init()
{
}

void BoundingBox::Update(float dt)
{
}

void BoundingBox::Destroy()
{
}

BoundingBox::~BoundingBox()
{
	IComponent::~IComponent();
}