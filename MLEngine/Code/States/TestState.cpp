#include "../States/TestState.h"

TestState::TestState()
{
	std::cout << "Test state has been constructed." << std::endl;
}

TestState::~TestState()
{
	std::cout << "Test state has been destructed." << std::endl;
}

void TestState::StateInit()
{
	std::cout << "Test state has been inited." << std::endl;
	
}

void TestState::StateDestroy()
{
	std::cout << "Test state has been destroyed." << std::endl;
	IState::StateDestroy();
}

bool TestState::Update(float dt)
{
	//Has to happen after everything
	std::cout << "State being updated with time: " << dt << std::endl;
	std::unique_ptr<TestState> newState = std::unique_ptr<TestState>(new TestState());
	std::cout << "Setting new state" << std::endl;
	SetNextState(std::move(newState));
	std::cout << "Setting state to be finished" << std::endl;
	SetStateFinished();
	return IState::Update(dt);
}