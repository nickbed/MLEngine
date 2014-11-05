#include "GraphicsManager.h"
#include "../Assert/Assert.h"

GraphicsManager::GraphicsManager()
{
	currentWindow = nullptr;
}

GraphicsManager::~GraphicsManager()
{
	glfwTerminate();
}

bool GraphicsManager::Init()
{
	int result = glfwInit();
	if(result != gl::TRUE_)
	{
		glfwTerminate();
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_ERROR, "Failed to init GLFW");
		return false;
	}
	return true;
}

bool GraphicsManager::CreateGraphicsWindow(const int xSize, const int ySize, const char* windowTitle)
{
	bool success = true;
	success &= mauveassert::Assert::AssertTrue("Can't create a window when a window already exists!",(currentWindow == nullptr), mauveassert::ENUM_severity::SEV_ERROR);
	success &= mauveassert::Assert::AssertTrue("Invalid X window size specified",(xSize > 1), mauveassert::ENUM_severity::SEV_ERROR);
	success &= mauveassert::Assert::AssertTrue("Invalid Y window size specified",(ySize > 1), mauveassert::ENUM_severity::SEV_ERROR);
	success &= mauveassert::Assert::AssertTrue("Null window title specified",(windowTitle != nullptr), mauveassert::ENUM_severity::SEV_ERROR);
	if(!success) return false;
	
	//Create window
	currentWindow = glfwCreateWindow(xSize, ySize, windowTitle, NULL, NULL);
	success &= mauveassert::Assert::AssertTrue("Window creation failed",(currentWindow != nullptr), mauveassert::ENUM_severity::SEV_ERROR);
	
	//Set up the context
	glfwMakeContextCurrent(currentWindow);

	if(!success) return false;
	return true;
}

bool GraphicsManager::DrawAndUpdateWindow()
{
	return true;
}

GLFWwindow* GraphicsManager::GetCurrentWindow()
{
	return currentWindow;
}

const int GraphicsManager::GetXSize()
{
	return xSize;
}

const int GraphicsManager::GetYSize()
{
	return ySize;
}