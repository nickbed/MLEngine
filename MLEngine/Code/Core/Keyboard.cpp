#include "Keyboard.h"

bool mauveinput::Keyboard::Keys[400];


mauveinput::Keyboard::Keyboard(){
	mRenderWindow = nullptr;
}

mauveinput::Keyboard::~Keyboard(){
	mRenderWindow = nullptr;
}

bool mauveinput::Keyboard::Initialize(GLFWwindow* window){
	mRenderWindow = window;

	DEBUGWRITEINFO("Reserving 400 spaces in keyboard buffer", "");
	for (int i = 0; i < 399; ++i)
	{
		Keys[i] = false;
	}
	if(window){
		glfwSetKeyCallback(this->mRenderWindow, keyCallback);
	} else {
		return false;
	}
	return true;
}

void mauveinput::Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN || key > 399)
	{
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_WARNING, "Key handler has rejected an invalid key");
		return;
	}
	Keys[key] = ((action == GLFW_PRESS || action == GLFW_REPEAT));
}

bool mauveinput::Keyboard::getAscii(std::string charcode){
	//converts and ascii key to Apex Key
	std::transform(charcode.begin(), charcode.end(), charcode.begin(), ::tolower);
	if (charcode.length() == 1)
	{
		char c = *charcode.c_str();
		int code = c;
		return Keys[code];
	}
	else {
		if (charcode == "up") return Keys[GLFW_KEY_UP];
		if (charcode == "down") return Keys[GLFW_KEY_DOWN];
		if (charcode == "left") return Keys[GLFW_KEY_LEFT];
		if (charcode == "right") return Keys[GLFW_KEY_RIGHT];
		if (charcode == "space") return Keys[GLFW_KEY_SPACE];
		if (charcode == "esc") return Keys[GLFW_KEY_ESCAPE];
	}
	return 0;
}

bool mauveinput::Keyboard::get(unsigned int keyvalue){
	if (keyvalue == GLFW_KEY_UNKNOWN || keyvalue > 399)
	{
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_WARNING, "Key handler has rejected an invalid key");
		return false;
	}
	return Keys[keyvalue];
}

bool mauveinput::Keyboard::Destroy(){
	if (this->mRenderWindow)
	{
		glfwSetKeyCallback(mRenderWindow, NULL);
		this->mRenderWindow = nullptr;
	}
	return true;
}
