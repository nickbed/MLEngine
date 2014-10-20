#include "../Systems/LogicSystem.h"
#include "../Components/DebugComponent.h"

LogicSystem::LogicSystem()
{
	Init();
}

LogicSystem::~LogicSystem()
{
}

void LogicSystem::Init()
{
	//Create some components
	std::unique_ptr<DebugComponent> debugComponent = std::unique_ptr<DebugComponent>(new DebugComponent("debug"));
	AddComponent(std::move(debugComponent));
	BaseMessage firstMessage = BaseMessage(MessageTypes::GeneralMessage);
	firstMessage.messageString = "Hello World!!";
	GetComponent("debug").SendMessage(firstMessage);
}

void LogicSystem::Update(float dt)
{
	ISystem::Update(dt);
	BaseMessage firstMessage = BaseMessage(MessageTypes::GeneralMessage);
	firstMessage.messageString = "Updating";
	GetComponent("debug").SendMessage(firstMessage);
}

void LogicSystem::Destroy()
{
}
