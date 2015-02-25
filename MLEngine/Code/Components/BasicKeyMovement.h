#ifndef BASICKEYMOVEMENT_H
#define BASICKEYMOVEMENT_H
#include "..\Assert\Assert.h"
#include "..\Interfaces\IComponent.h"
#include "..\Messages\PositionMessage.h"
#include "..\Messages\MessageManager.h"
#include "GLFW\glfw3.h"
#include <map>

#include "../Core/Keyboard.h"

class BasicKeyMovement : public IComponent
{
public:
	BasicKeyMovement(std::string id, GLFWwindow* win);

	virtual void Init();

	virtual void Update(float dt);

	virtual void Destroy();

	virtual ~BasicKeyMovement();

private:
	//The window that will be used to handle input events
	GLFWwindow* window;

	void SendMovementMessage(glm::vec3 msg, const char* messageType);
	bool used;
};

#endif