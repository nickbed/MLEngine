#ifndef Entity_H
#define Entity_H
#include <vector>
#include <string>
#include <memory>
#include "..\Components\ComponentManager.h"
#include "..\Components\Transform.h"

//Interface (Abstract class) that defines how components interact with entities
class IEntity
{
public:

	IEntity();
	
	//Pass in a pre made list of components
	IEntity(std::unique_ptr<componentMapType> componentList);
	virtual ~IEntity();

	virtual void Init();

	//Update method - updates Entity and all components
	virtual bool Update(float dt);

	//Gets called to bin us
	virtual void Destroy();



	//List of components
	ComponentManager* Components;

	//EVERYTHING has a transform
	TransformComponent* Transform;
};

//Useful macro - Default implementation of calling the base class and moving the pointer if a map is specified
#define DEFAULTENTITYCONSTRUCTOR(type) type() : IEntity(){Init();}; type(std::unique_ptr<componentMapType> componentList) : IEntity(std::move(componentList)){Init();};

#endif