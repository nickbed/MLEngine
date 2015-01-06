#include <memory>
#include <thread>
#include "Assert\Assert.h"
#include "Core/Engine.h"
#include "Messages\MessageManager.h"

void ExceptionHandler(const char* message);
int guardedMain();


int main()
{
	//mauveassert::Assert assertManager;
	//mauvemessage::MessageManager messageManager;
	mauveassert::Assert::InitExceptionHandler(&ExceptionHandler);
	try{
		guardedMain();
	}
	catch(std::exception e)
	{
		mauveassert::Assert::HandleException(e.what());
	}

	DEBUGWRITEINFO("Closed with no errors", "");
	system("pause");
	return 0;
}

int guardedMain()
{
	//Do our config
	EngineConfig currentConf;
	currentConf.resX = 1024;
	currentConf.resY = 768;
	float sixtyFPS = (1.0f / 90.0f) * 1000;

	//Make our engine
	Engine currentEngine = Engine();
	currentConf = currentEngine.ReadConfigFile("data\\config\\EngineConfig.json");
	currentEngine.Init(currentConf);
	while(true)
	{
		long timeToWait = sixtyFPS - (glfwGetTime() * 1000);
		if (timeToWait <= 0)
		{
			if (!currentEngine.Update(glfwGetTime()))
			{
				break;
			}
			glfwSetTime(0.0f);
		}
		else
		{
			std::this_thread::yield();
		}
	}
	return 0;
}

void ExceptionHandler(const char* message)
{
	//Just close it
	std::cout << "Handling an exception :-)" << message << std::endl;
	system("pause");
	std::exit(1);
}