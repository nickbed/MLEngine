#include "../Interfaces/IState.h"

IState::IState()
{
	stateFinished = false;
}

IState::~IState()
{
}

void IState::StateInit()
{
}

void IState::StateDestroy()
{
	//Kill it
	this->~IState();
}

std::unique_ptr<IState> IState::GetNextState()
{
	//Give ownership to the caller
	return std::move(nextState);
}

bool IState::Update(float dt)
{
	if(stateFinished)
	{
		return false;
	}
	return true;
}

void IState::SetNextState(std::unique_ptr<IState> newState)
{
	this->nextState = std::move(newState);
}

void IState::SetStateFinished()
{
	this->stateFinished = true;
}
