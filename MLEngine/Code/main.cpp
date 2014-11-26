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


	system("pause");
	return 0;
}

int guardedMain()
{
	//Do our config
	EngineConfig currentConf;
	currentConf.resX = 1024;
	currentConf.resY = 768;
	float sixtyFPS = 1.0f / 60.0f;

	//Make our engine
	Engine currentEngine = Engine(currentConf);
	currentEngine.Init();
	while(true)
	{
		if (glfwGetTime() > sixtyFPS)
		{
			if (!currentEngine.Update(glfwGetTime()))
			{
				break;
			}
			glfwSetTime(0.0f);
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