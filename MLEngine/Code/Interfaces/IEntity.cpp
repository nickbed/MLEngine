#include "IEntity.h"
#include "..\Assert\Assert.h"

IEntity::IEntity()
{
	//If no component list is specified, create it here.
	std::unique_ptr<componentMapType> components(new componentMapType);
	Components = new ComponentManager(std::move(components));
	
	Transform = new TransformComponent("defaultTransform");
}

IEntity::IEntity(std::unique_ptr<componentMapType> componentList)
{
	Components = new ComponentManager(std::move(componentList));
	
	Transform = new TransformComponent("defaultTransform");
}

IEntity::~IEntity()
{
	if(Components != nullptr) delete Components;
	if(Transform != nullptr) delete Transform;
}
