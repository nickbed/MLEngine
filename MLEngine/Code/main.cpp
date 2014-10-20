#include <memory>
#include <thread>
#include "Core/Engine.h"

int main()
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

	system("pause");
	return 0;
}