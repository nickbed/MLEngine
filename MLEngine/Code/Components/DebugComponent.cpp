#include "DebugComponent.h"

DebugComponent::DebugComponent(std::string id) : IComponent(id)
{
	std::cout << "Debug Component has been Inited with ID: " << id << std::endl;
}

void DebugComponent::Init()
{
	std::cout << "Debug Component has been Inited!!" << std::endl;
}

void DebugComponent::Update(double dt)
{
	std::cout << "Debug Component has been updated with delta t: " << dt << std::endl;
}

void DebugComponent::Destroy()
{
	std::cout << "Debug Component has been destroyed" << std::endl;
}

DebugComponent::~DebugComponent()
{
}

