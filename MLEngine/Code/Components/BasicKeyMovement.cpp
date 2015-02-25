#include "BasicKeyMovement.h"

BasicKeyMovement::BasicKeyMovement(std::string id, GLFWwindow* win) : window(win), IComponent(id) 
{
}

void BasicKeyMovement::Init()
{
	used = false;
}

void BasicKeyMovement::Update(float dt)
{
	//W == 87
	//A == 65
	//S == 83
	//D == 68
	float movementConst = 10.0f;
	if (mauveinput::Keyboard::get(GLFW_KEY_LEFT_SHIFT))
	{
		movementConst *= 2;
	}
	//Up
	if (mauveinput::Keyboard::get(GLFW_KEY_W))
	{
		//Forward in Z
		SendMovementMessage(glm::vec3(0.0f, 0.0f, movementConst * dt), "cameraMovement");
	}

	//Down
	if (mauveinput::Keyboard::get(GLFW_KEY_S))
	{
		//Backwards in Z
		SendMovementMessage(glm::vec3(0.0f, 0.0f, -movementConst * dt), "cameraMovement");
	}

	//Left
	if (mauveinput::Keyboard::get(GLFW_KEY_A))
	{
		//Left in X
		SendMovementMessage(glm::vec3(-movementConst * dt, 0.0f, 0.0f), "cameraMovement");
	}

	//Right
	if (mauveinput::Keyboard::get(GLFW_KEY_D))
	{
		//Right in X
		SendMovementMessage(glm::vec3(movementConst * dt, 0.0f, 0.0f), "cameraMovement");
	}

	//Robot
	//Up
	if (mauveinput::Keyboard::get(GLFW_KEY_UP))
	{
		//Forward in Z
		SendMovementMessage(glm::vec3(0.0f, 0.0f, movementConst * dt), "robotMovement");
	}

	//Down
	if (mauveinput::Keyboard::get(GLFW_KEY_DOWN))
	{
		//Backwards in Z
		SendMovementMessage(glm::vec3(0.0f, 0.0f, -movementConst * dt), "robotMovement");
	}

	//Left
	if (mauveinput::Keyboard::get(GLFW_KEY_LEFT))
	{
		//Left in X
		SendMovementMessage(glm::vec3(-movementConst * dt, 0.0f, 0.0f), "robotMovement");
	}

	//Right
	if (mauveinput::Keyboard::get(GLFW_KEY_RIGHT))
	{
		//Right in X
		SendMovementMessage(glm::vec3(movementConst * dt, 0.0f, 0.0f), "robotMovement");
	}

	//Camera 1
	if (mauveinput::Keyboard::get(GLFW_KEY_1))
	{
		//Right in X
		SendMovementMessage(glm::vec3(1.0f, 1.0f, 1.0f), "setCamera");
	}

	//camera 2
	if (mauveinput::Keyboard::get(GLFW_KEY_2))
	{
		//Right in X
		SendMovementMessage(glm::vec3(2.0f, 2.0f, 2.0f), "setCamera");
	}

	//camera 3
	if (mauveinput::Keyboard::get(GLFW_KEY_3))
	{
		//Right in X
		SendMovementMessage(glm::vec3(3.0f, 3.0f, 3.0f), "setCamera");
	}

	//camera 3
	if (mauveinput::Keyboard::get(GLFW_KEY_4))
	{
		//Right in X
		SendMovementMessage(glm::vec3(4.0f, 4.0f, 4.0f), "setCamera");
	}

	//reload scene
	if (mauveinput::Keyboard::get(GLFW_KEY_R))
	{
		//Right in X
		SendMovementMessage(glm::vec3(4.0f, 4.0f, 4.0f), "reloadScene");
	}

	if (mauveinput::Keyboard::get(GLFW_KEY_O))
	{
		SendMovementMessage(glm::vec3(1.0f,1.0f,1.0f), "showDebug");
	}

	if (mauveinput::Keyboard::get(GLFW_KEY_P))
	{
		SendMovementMessage(glm::vec3(0.0f,0.0f,0.0f), "showDebug");
	}

	if (mauveinput::Keyboard::get(GLFW_KEY_F1))
	{
		SendMovementMessage(glm::vec3(0.0f, 0.0f, 0.0f), "loadGame");
	}
}

void BasicKeyMovement::SendMovementMessage(glm::vec3 msg, const char* movementName)
{
	
	mauvemessage::PositionMessage posMsg = mauvemessage::PositionMessage(movementName, msg);
	mauvemessage::MessageManager::SendListnerMessage(&posMsg, movementName);
}

void BasicKeyMovement::Destroy()
{
}

BasicKeyMovement::~BasicKeyMovement()
{
	Destroy();
}