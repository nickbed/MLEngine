#include "BasicKeyMovement.h"

bool BasicKeyMovement::keyBuffer[400];

BasicKeyMovement::BasicKeyMovement(std::string id, GLFWwindow* win) : window(win), IComponent(id) 
{
}

void BasicKeyMovement::Init()
{
	//Setup callback here
	glfwSetKeyCallback(window, &BasicKeyMovement::keyFunc);
	
	DEBUGWRITEINFO("Reserving 400 spaces in keyboard buffer", "");
	for (int i = 0; i < 399; ++i)
	{
		keyBuffer[i] = false;
	}
	used = false;
}

void BasicKeyMovement::Update(float dt)
{
	//W == 87
	//A == 65
	//S == 83
	//D == 68
	float movementConst = 10.0f;

	//Up
	if (keyBuffer[GLFW_KEY_W])
	{
		//Forward in Z
		SendMovementMessage(glm::vec3(0.0f, 0.0f, movementConst * dt), "cameraMovement");
	}

	//Down
	if (keyBuffer[GLFW_KEY_S])
	{
		//Backwards in Z
		SendMovementMessage(glm::vec3(0.0f, 0.0f, -movementConst * dt), "cameraMovement");
	}

	//Left
	if (keyBuffer[GLFW_KEY_A])
	{
		//Left in X
		SendMovementMessage(glm::vec3(-movementConst * dt, 0.0f, 0.0f), "cameraMovement");
	}

	//Right
	if (keyBuffer[GLFW_KEY_D])
	{
		//Right in X
		SendMovementMessage(glm::vec3(movementConst * dt, 0.0f, 0.0f), "cameraMovement");
	}

	//Robot
	//Up
	if (keyBuffer[GLFW_KEY_UP])
	{
		//Forward in Z
		SendMovementMessage(glm::vec3(0.0f, 0.0f, movementConst * dt), "robotMovement");
	}

	//Down
	if (keyBuffer[GLFW_KEY_DOWN])
	{
		//Backwards in Z
		SendMovementMessage(glm::vec3(0.0f, 0.0f, -movementConst * dt), "robotMovement");
	}

	//Left
	if (keyBuffer[GLFW_KEY_LEFT])
	{
		//Left in X
		SendMovementMessage(glm::vec3(-movementConst * dt, 0.0f, 0.0f), "robotMovement");
	}

	//Right
	if (keyBuffer[GLFW_KEY_RIGHT])
	{
		//Right in X
		SendMovementMessage(glm::vec3(movementConst * dt, 0.0f, 0.0f), "robotMovement");
	}

	//Camera 1
	if (keyBuffer[GLFW_KEY_1])
	{
		//Right in X
		SendMovementMessage(glm::vec3(1.0f, 1.0f, 1.0f), "setCamera");
	}

	//camera 2
	if (keyBuffer[GLFW_KEY_2])
	{
		//Right in X
		SendMovementMessage(glm::vec3(2.0f, 2.0f, 2.0f), "setCamera");
	}

	//camera 3
	if (keyBuffer[GLFW_KEY_3])
	{
		//Right in X
		SendMovementMessage(glm::vec3(3.0f, 3.0f, 3.0f), "setCamera");
	}

	//camera 3
	if (keyBuffer[GLFW_KEY_4])
	{
		//Right in X
		SendMovementMessage(glm::vec3(4.0f, 4.0f, 4.0f), "setCamera");
	}

	//reload scene
	if (keyBuffer[GLFW_KEY_R])
	{
		//Right in X
		SendMovementMessage(glm::vec3(4.0f, 4.0f, 4.0f), "reloadScene");
	}

	if (keyBuffer[GLFW_KEY_O])
	{
		SendMovementMessage(glm::vec3(1.0f,1.0f,1.0f), "showDebug");
	}

	if (keyBuffer[GLFW_KEY_P])
	{
		SendMovementMessage(glm::vec3(0.0f,0.0f,0.0f), "showDebug");
	}

	//Test message!
	if (keyBuffer[399])
	{
		SendMovementMessage(glm::vec3(1.0, 1.0, 1.0), "keyboardMovement");
		keyBuffer[399] = false;
	}
}

void BasicKeyMovement::SendMovementMessage(glm::vec3 msg, const char* movementName)
{
	
	mauvemessage::PositionMessage posMsg = mauvemessage::PositionMessage(movementName, msg);
	mauvemessage::MessageManager::SendListnerMessage(&posMsg, movementName);
}

void BasicKeyMovement::Destroy()
{
	//Unbind callback here
	glfwSetKeyCallback(window, NULL);
}

void BasicKeyMovement::TestMessage()
{
	keyBuffer[399] = true;
}

BasicKeyMovement::~BasicKeyMovement()
{
	Destroy();
}

void BasicKeyMovement::keyFunc(GLFWwindow* win, int key, int scanCode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN || key > 399)
	{
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_WARNING, "Key handler has rejected an invalid key");
		return;
	}
	keyBuffer[key] = ((action == GLFW_PRESS || action == GLFW_REPEAT));
}