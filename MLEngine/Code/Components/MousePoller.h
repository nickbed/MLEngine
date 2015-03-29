#ifndef MOUSEPOLLER_H
#define MOUSEPOLLER_H

#include "..\Assert\Assert.h"
#include "..\Interfaces\IComponent.h"
#include "GLFW\glfw3.h"
#include "..\Messages\MessageManager.h"
#include "..\Messages\PositionMessage.h"

class MousePoller : public IComponent
{
public:
	MousePoller(const char* compId, GLFWwindow* win);
	~MousePoller();
	
	void Init();

	void Update(float dt);
	
	void Destroy();

private:
	GLFWwindow* window;
	int halfWindowHeight;
	int halfWindowWidth;
	double oldMousePosX;
	double oldMousePosY;
	bool enabled;
};

#endif
