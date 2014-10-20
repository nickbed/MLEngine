#ifndef SYSTEM_H
#define SYSTEM_H
#include <vector>
#include <string>
#include <memory>
#include "IComponent.h"

//Interface (Abstract class) that defines how components interact with entities
class ISystem
{
public:
	virtual void Init();

	//Update method - updates system and all components
	virtual void Update(float dt) = 0;

	//Gets called to bin us
	virtual void Destroy() = 0;

	//Destructor
	virtual ~ISystem();

	//Get component with specified id
	IComponent& GetComponent(std::string id);

	//Returns the existence of the component with specified ID
	bool HasComponent(std::string id);

	//Adds a new component
	void AddComponent(std::unique_ptr<IComponent> component);

private:
	std::vector<std::unique_ptr<IComponent>> Components;
};

#endif