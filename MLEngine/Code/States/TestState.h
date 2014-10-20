#ifndef TESTSTATE_H
#define TESTSTATE_H
#include "../Interfaces/IState.h"
#include <iostream>

class TestState : public IState
{
public:
	TestState();
	virtual ~TestState();
	void StateInit();
	void StateDestroy();
	bool Update(float dt);
};

#endif