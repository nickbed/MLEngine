#include "../Interfaces/IComponent.h"
//Implementation of the abstract class

IComponent::IComponent(std::string id)
{
	this->id = id;
}

void IComponent::Update(float dt)
{
}

//void IComponent::SendMessage(BaseMessage message)
//{
//}

void IComponent::Destroy()
{
}

IComponent::~IComponent()
{
}
