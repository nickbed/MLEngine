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
}

void BasicKeyMovement::Update(float dt)
{
	//W == 87
	//A == 65
	//S == 83
	//D == 68
	float movementConst = 2.0f;

	//Up
	if (keyBuffer[GLFW_KEY_W])
	{
		//Forward in Z
		SendMovementMessage(glm::vec3(0.0f, 0.0f, movementConst * dt));
	}

	//Down
	if (keyBuffer[GLFW_KEY_S])
	{
		//Backwards in Z
		SendMovementMessage(glm::vec3(0.0f, 0.0f, -movementConst * dt));
	}

	//Left
	if (keyBuffer[GLFW_KEY_A])
	{
		//Left in X
		SendMovementMessage(glm::vec3(movementConst * dt, 0.0f, 0.0f));
	}

	//Right
	if (keyBuffer[GLFW_KEY_D])
	{
		//Right in X
		SendMovementMessage(glm::vec3(-movementConst * dt, 0.0f, 0.0f));
	}

	//Test message!
	if (keyBuffer[399])
	{
		SendMovementMessage(glm::vec3(1.0, 1.0, 1.0));
		keyBuffer[399] = false;
	}
}

void BasicKeyMovement::SendMovementMessage(glm::vec3 msg)
{
	mauvemessage::PositionMessage posMsg = mauvemessage::PositionMessage("keyboardMovement", msg);
	mauvemessage::MessageManager::SendListnerMessage(&posMsg, "keyboardMovement");
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