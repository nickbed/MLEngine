#ifndef ISTATE_H
#define ISTATE_H
#include <memory>
class IState
{
public:
	IState();
	virtual ~IState();
	
	//For to be called when the state is inited or destroyed
	virtual void StateInit() = 0;

	//Not pure virtual because we want some functionality in the base class
	virtual void StateDestroy();

	//Gets a unique ptr to the next state, intended to be moved to ownership by the parent class once needed
	std::unique_ptr<IState> GetNextState();
	void SetNextState(std::unique_ptr<IState> newState);
	
	//When state is ready to move to next state, this returns false
	virtual bool Update(float dt);
	void SetStateFinished();

private:
	std::unique_ptr<IState> nextState;
	bool stateFinished;
};

#endif