#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H
#include "..\Components\MessageKeyMovement.h"
#include "..\Components\MousePoller.h"
#include "..\Interfaces\IEntity.h"
#include "GLFW\glfw3.h"

class InputSystem : public IEntity
{
public:

	DEFAULTENTITYCONSTRUCTOR(InputSystem);

	~InputSystem();

	void Init(GLFWwindow* inputWindow);

	void Init();

	bool Update(float dt);

	void Destroy();

private:
	IEntity* inputComponent;
	MessageKeyMovement* currentKeyboard;
	MousePoller* currentMouse;
};

#endif