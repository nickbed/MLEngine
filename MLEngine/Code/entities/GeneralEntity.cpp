#include "GeneralEntity.h"

GeneralEntity::~GeneralEntity()
{
}

void GeneralEntity::Init()
{
	IEntity::Init();
}

bool GeneralEntity::Update(float dt)
{
	Components->UpdateAllComponents(dt);
	return true;
}

void GeneralEntity::Destroy()
{
	IEntity::Destroy();
}

