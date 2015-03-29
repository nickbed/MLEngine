#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <thread>
#include "Assert\Assert.h"
#include "Core/Engine.h"
#include "Messages\MessageManager.h"
#include "Resources\GPUResourceManager.h"
#include "Resources\ResourceManager.h"


void ExceptionHandler(const char* message);
int guardedMain();


int main()
{
	mauveassert::Assert::InitExceptionHandler(&ExceptionHandler);
	try{
		guardedMain();
	}
	catch(std::exception e)
	{
		mauveassert::Assert::HandleException(e.what());
	}
#ifdef _DEBUG
	DEBUGWRITEINFO("Closed with no errors", "");
	DEBUGWRITEINFO("Press any key to continue...", "");
	std::cin.get();
#endif
	return 0;
}

int guardedMain()
{
	//Do our config
	EngineConfig currentConf;
	currentConf.resX = 1024;
	currentConf.resY = 768;


	float sixtyFPS = (1.0f / 60.0f);

	double oldTime = 0; 
	double oldTime2 = 0;
	bool closed = false;

	//Make our engine
	Engine currentEngine = Engine();
	currentConf = currentEngine.ReadConfigFile("data\\config\\EngineConfig.json");
	currentEngine.Init(currentConf);
	float timeToWait = 0.0;
	float delta = 0.0f;
	double timeTaken = 0.0;
	while(true)
	{
		if (timeTaken >= sixtyFPS)
		{
			delta = (float)(glfwGetTime() - oldTime);
			oldTime = glfwGetTime();
			timeTaken = 0.0;
			if (!currentEngine.Update(delta))
			{
				closed = true;
				break;
			}
		}
		else
		{
			timeTaken += glfwGetTime() - oldTime2;
			oldTime2 = glfwGetTime();
		}
		if (closed) break;
		currentEngine.Draw();


		//else
		//{
		//	std::this_thread::yield();
		//}
	}
	//_CrtDumpMemoryLeaks();
	return 0;
}

void ExceptionHandler(const char* message)
{
	//Just close it
#ifdef _DEBUG 
	std::cout << "Handling an exception :-)" << message << std::endl;
	system("pause");
#else
	std::stringstream s;
	s << "I'm sorry but I've had to quit due to an error :-(" << std::endl <<std::endl << "Details: " << message;
	MessageBox(
		NULL,
		s.str().c_str(),
		"Mauve lamp has fallen..",
		MB_ICONSTOP
		);
#endif

	std::exit(1);
}