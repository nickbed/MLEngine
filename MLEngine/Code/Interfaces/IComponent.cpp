#include "../Interfaces/IComponent.h"
//Implementation of the abstract class

IComponent::IComponent(std::string id)
{
	this->id = id;
}

void IComponent::Update(float dt)
{
}

void IComponent::Destroy()
{
}

TransformComponent* IComponent::GetParentTransform()
{
	return parentTransform;
}

void IComponent::SetParentTransform(TransformComponent* pTransform)
{
	parentTransform = pTransform;
}

IComponent::~IComponent()
{
}