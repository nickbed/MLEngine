#include "GeneralEntity.h"

GeneralEntity::~GeneralEntity()
{
	IEntity::~IEntity();
}

void GeneralEntity::Init()
{
}

bool GeneralEntity::Update(float dt)
{
	return false;
}

void GeneralEntity::Destroy()
{
}

