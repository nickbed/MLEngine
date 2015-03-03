#include "InputSystem.h"

void InputSystem::Init()
{
}

void InputSystem::Init(GLFWwindow* inputWindow)
{
	//create components, init them and add them to the enity;
	MessageKeyMovement* movement = new MessageKeyMovement("keyboardMovement", inputWindow);
	movement->Init();

	MousePoller* mouseReader = new MousePoller("mouseMovement", inputWindow);
	mouseReader->Init();
	IEntity::Components->AddComponent("keyboard", movement);
	IEntity::Components->AddComponent("mouse", mouseReader);
}

bool InputSystem::Update(float dt)
{
	IEntity::Update(dt);
	return true;
}

void InputSystem::Destroy()
{
}
