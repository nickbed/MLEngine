#include "InputSystem.h"

InputSystem::~InputSystem()
{
	if(currentKeyboard != nullptr) currentKeyboard->Destroy();
	if(currentMouse != nullptr) currentMouse->Destroy();
}


void InputSystem::Init()
{
	currentMouse = nullptr;
	currentKeyboard = nullptr;
}

void InputSystem::Init(GLFWwindow* inputWindow)
{
	//create components, init them and add them to the enity;
	MessageKeyMovement* movement = new MessageKeyMovement("keyboardMovement", inputWindow);
	movement->Init();
	currentKeyboard = movement;

	MousePoller* mouseReader = new MousePoller("mouseMovement", inputWindow);
	mouseReader->Init();
	currentMouse = mouseReader;
	IEntity::Components->AddComponent("keyboard", movement);
	IEntity::Components->AddComponent("mouse", mouseReader);
}

bool InputSystem::Update(double dt)
{
	IEntity::Update(dt);
	return true;
}

void InputSystem::Destroy()
{

}
