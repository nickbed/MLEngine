#include "MessageKeyMovement.h"


bool MessageKeyMovement::keyBuffer[400];
bool MessageKeyMovement::used;

MessageKeyMovement::MessageKeyMovement(std::string id, GLFWwindow* win) : window(win), IComponent(id)
{
}

void MessageKeyMovement::Init()
{
	//Setup callback here
	glfwSetKeyCallback(window, &MessageKeyMovement::keyFunc);

	DEBUGWRITEINFO("Reserving 400 spaces in keyboard buffer", "");
	for (int i = 0; i < 399; ++i)
	{
		keyBuffer[i] = false;
	}
	used = false;
}

void MessageKeyMovement::Update(float dt)
{
	if (!used) return;
	for (int i = 0; i < 399; ++i)
	{
		if (keyBuffer[i])
		{
			SendKeyMessage(i, "keyPressed");
		}
	}
	used = false;
}

void MessageKeyMovement::SendKeyMessage(int msg, const char* messageName)
{

	mauvemessage::KeyMessage posMsg = mauvemessage::KeyMessage(messageName, msg);
	mauvemessage::MessageManager::SendListnerMessage(&posMsg, messageName);
}

void MessageKeyMovement::Destroy()
{
	//Unbind callback here
	glfwSetKeyCallback(window, NULL);
}

void MessageKeyMovement::TestMessage()
{
	keyBuffer[399] = true;
}

MessageKeyMovement::~MessageKeyMovement()
{
	Destroy();
}

void MessageKeyMovement::keyFunc(GLFWwindow* win, int key, int scanCode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN || key > 399)
	{
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_WARNING, "Key handler has rejected an invalid key");
		return;
	}
	keyBuffer[key] = ((action == GLFW_PRESS || action == GLFW_REPEAT));
	used = true;
}