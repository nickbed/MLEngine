#include "Engine.h"
#include "../States/TestState.h"

Engine::Engine(const EngineConfig conf) : currentConfig(conf)
{
}

Engine::~Engine()
{
	currentState.release();
}

void Engine::Init()
{
	loops = 0;
	//Load the config in
	std::unique_ptr<TestState> firstState = std::unique_ptr<TestState>(new TestState());
	this->currentState = std::move(firstState);
}

bool Engine::Update(float dt)
{
	++loops;
	if(loops > 10)
	{
		return false;
	}

	if(!this->currentState->Update(10.0f))
	{
		if(!LoadNextState())
		{
			//do something else
			return false;
		}
	}

	return true;
}

bool Engine::LoadNextState()
{
	//Cache our next state so it doesn't get destroyed
	std::unique_ptr<IState> nextState = std::move(this->currentState->GetNextState());
	
	//Check to see if we have a next state, if we don't then handle it elsewhere in the engine.
	if(nextState == nullptr)
	{
		//handle this in our update loop
		return false;
	}

	//Clean up our old current state
	this->currentState->StateDestroy();
	this->currentState.release();

	//Plop our new state into 
	this->currentState = std::move(nextState);

	//Init our new state
	this->currentState->StateInit();

	//Success
	return true;
}

