#ifndef BASICKEYMOVEMENT_H
#define BASICKEYMOVEMENT_H
#include "..\Assert\Assert.h"
#include "..\Interfaces\IComponent.h"
#include "..\Messages\PositionMessage.h"
#include "..\Messages\MessageManager.h"
#include "GLFW\glfw3.h"
#include <map>

class BasicKeyMovement : public IComponent
{
public:
	BasicKeyMovement(std::string id, GLFWwindow* win);

	virtual void Init();

	virtual void Update(float dt);

	virtual void Destroy();

	//Sends a test message
	void TestMessage();

	virtual ~BasicKeyMovement();

private:
	//The window that will be used to handle input events
	GLFWwindow* window;
	static void keyFunc(GLFWwindow* win, int key, int scanCode, int action, int mods);
	static bool keyBuffer[400];  //1->1 mapping with keycodes
	void SendMovementMessage(glm::vec3 msg);
};

#endif