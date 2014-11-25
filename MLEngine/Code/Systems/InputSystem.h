#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H
#include "..\Interfaces\IEntity.h"

class InputSystem : public IEntity
{
public:

	DEFAULTENTITYCONSTRUCTOR(InputSystem);

	void Init();

	bool Update(float dt);

	void Destroy();
};

#endif