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
	currentConf.resX = 640;
	currentConf.resY = 480;

	//Make our engine
	Engine currentEngine = Engine(currentConf);
	currentEngine.Init();
	while(true)
	{
		if(!currentEngine.Update())
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
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