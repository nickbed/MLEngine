#include "IEntity.h"
#include "..\Messages\MessageManager.h"
#include "..\Assert\Assert.h"

IEntity::IEntity()
{
	//If no component list is specified, create it here.
	std::unique_ptr<componentMapType> components(new componentMapType, std::default_delete<componentMapType>());
	Transform = new TransformComponent("defaultTransform");
	Script = new ScriptComponent("defaultScript");
	Script->owner = this;
	Components = new ComponentManager(std::move(components), this->Transform);
}

IEntity::IEntity(std::unique_ptr<componentMapType> componentList)	
{
	Transform = new TransformComponent("defaultTransform");
	Script = new ScriptComponent("defaultScript");
	Script->owner = this;
	Components = new ComponentManager(std::move(componentList), this->Transform);
}

bool IEntity::Update(float dt)
{
	Components->UpdateAllComponents(dt);
	Transform->Update(dt);

	Script->Update(dt);
	return true;
}

void IEntity::Init()
{
	Components->InitAllComponents();
	Transform->Init();

	//Script->Init();
}

void IEntity::Destroy()
{
	Components->DestroyAllComponents();
	Transform->Destroy();
	Script->Destroy();
	//Remove us as a listner for any messages we might be listening to.
	mauvemessage::MessageManager::ClearMessageListner(this);
}

IEntity::~IEntity()
{
	if(Components != nullptr) delete Components;
	if (Transform != nullptr) delete Transform;
}

void IEntity::setTransform(TransformComponent* t)
{
	Transform = t;
}
