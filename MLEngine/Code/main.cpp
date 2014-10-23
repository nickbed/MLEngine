#include <memory>
#include <thread>
#include "Core/Engine.h"
#include "Assert\Assert.h"
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
	currentConf.resX = 0;
	currentConf.resY = 0;

	//Make our engine
	Engine currentEngine = Engine(currentConf);
	currentEngine.Init();
	while(true)
	{
		if(!currentEngine.Update(10.0f))
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
	return 0;
}

void ExceptionHandler(const char* message)
{
	//Just close it
	std::cout << "Handling an exception :-)" << std::endl;
	system("pause");
	std::exit(1);
}