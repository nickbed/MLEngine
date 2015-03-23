#include "MousePoller.h"

MousePoller::MousePoller(const char* compID, GLFWwindow* win) : IComponent(compID)
{
	NULLPTRCHECK(win, "Window passed to mouse poller component is null!!");
	NULLPTRCHECK(win, "ID passed to mouse poller component is null!!");
	window = win;
}

MousePoller::~MousePoller()
{
	Destroy();
}

void MousePoller::Init()
{
	int windowHeight;
	int windowWidth;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	halfWindowHeight = windowHeight / 2;
	halfWindowWidth = windowWidth / 2;
}

void MousePoller::Update(float dt)
{
	NULLPTRCHECK(window, "Mouse poller called update on null window!");
	//Could use a callback to read the mouse every time it moves, but the position will only be read once per update anyway
	double mouseXPos = 0.0;
	double mouseYPos = 0.0;
	float mouseSpeed = 0.05f;

	//We get how far the cursor has moved in the frame, then reset it.
	glfwGetCursorPos(window, &mouseXPos, &mouseYPos);

	//How much has it moved in this time period relative to the window?
	float mouseXPosToSend = mouseSpeed * dt * (halfWindowWidth - (float)mouseXPos);
	float mouseYPosToSend = mouseSpeed * dt * (halfWindowHeight - (float)mouseYPos);

	glfwSetCursorPos(window, halfWindowWidth, halfWindowHeight);

	//Generate our mouse position message

	mauvemessage::PositionMessage mouseMessage("mouseMovement", glm::vec3(mouseXPosToSend, mouseYPosToSend, 0.0f));
	mauvemessage::MessageManager::SendListnerMessage(&mouseMessage, "mouseMovement");
}

void MousePoller::Destroy()
{

}