#include "DebugComponent.h"

DebugComponent::DebugComponent(std::string id) : IComponent(id)
{
	std::cout << "Debug Component has been Inited with ID: " << id << std::endl;
}

void DebugComponent::Init()
{
	std::cout << "Debug Component has been Inited!!" << std::endl;
}

void DebugComponent::Update(float dt)
{
	std::cout << "Debug Component has been updated with delta t: " << dt << std::endl;
}

void DebugComponent::SendMessage(BaseMessage message)
{
	std::string messageType = "Unknown";
	std::cout << "Debug Component has been sent the message: ";
	switch (message.id)
	{
	case MessageTypes::GeneralMessage:
		messageType = "GeneralMessage Data:" + message.messageString;
		break;
	}
	std::cout << messageType << std::endl;
}

void DebugComponent::Destroy()
{
	std::cout << "Debug Component has been destroyed" << std::endl;
}

DebugComponent::~DebugComponent()
{
}

