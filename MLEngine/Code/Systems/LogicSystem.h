#ifndef LOGICSYSTEM_H
#define LOGICSYSTEM_H
#include "../Interfaces/ISystem.h"

class LogicSystem : public ISystem
{
public: 
	LogicSystem();

	virtual ~LogicSystem();

	virtual void Init();

	//Update method - updates system and all components
	virtual void Update(float dt);

	//Gets called to bin us
	virtual void Destroy();
};

#endif
