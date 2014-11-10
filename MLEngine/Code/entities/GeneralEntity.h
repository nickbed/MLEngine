#ifndef GENERALENTITY_H
#define GENERALENTITY_H
#include "..\Interfaces\IEntity.h"

class GeneralEntity : public IEntity
{
public:
	DEFAULTENTITYCONSTRUCTOR(GeneralEntity);
	virtual ~GeneralEntity();
	
	virtual void Init();
	virtual bool Update(float dt);
	virtual void Destroy();

private:
};

#endif